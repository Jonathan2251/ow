   
#define ICON_WIDTH 15  // gamma
#define ICON_HEIGHT 13  // gamma

Line::Line(int x, int y) : length(0), blink(false) {
    for (int i = 0; i < 16; i++) {
        pos[i] = new CChar(x+i*10, y, 10, 14); // gamma:need to change when porting a new handset image.
    }
};
        
SymbolPanel::SymbolPanel(Line *line[2]) {
    for (int i = 0; i < 7; i++) {
        ch[i] = new TStaticText(Form1);
        ch[i]->Parent = Form1;
        ch[i]->Font->Size = line[0]->pos[i]->ch->Font->Size;
        ch[i]->Alignment = taCenter;
        ch[i]->Left = line[0]->pos[2*i]->ch->Left;
        ch[i]->Top = line[0]->pos[i]->ch->Top;
        ch[i]->Width = 2*line[0]->pos[i]->ch->Width;
        ch[i]->Height = line[0]->pos[i]->ch->Height;
        ch[i]->Color = TColor(0x008BBB89); // gamma:need to change when porting a new handset image.
        ch[i]->Visible = false;
    }
    size = 7; // necessary
};


// gamma:need to change when porting a new handset image.
Lcd::Lcd(PTConfig *aCfg) {
    line[0] = new Line(56, 154);// gamma:need to change when porting a new handset image.
    line[1] = new Line(56, 172);// gamma:need to change when porting a new handset image.
    panel = new SymbolPanel(line);

    for (int i = 0; i < BELL; i++) {
        icon[i] = new Icon;
        icon[i]->image->Left = line[1]->pos[0]->ch->Left + ICON_WIDTH*i;
        icon[i]->image->Top = line[1]->pos[0]->ch->Top + 22;
        icon[i]->image->Width = ICON_WIDTH;
        icon[i]->image->Height = ICON_HEIGHT;
        icon[i]->image->Transparent = true;
        icon[i]->image->Stretch = true;
        icon[i]->image->Visible = false;
    }
    icon[FUNCTION_SETTING]->image->Picture->LoadFromFile("bmp\\icon_function_setting.bmp");
    icon[TAM]->image->Picture->LoadFromFile("bmp\\icon_TAM.bmp");
    icon[LINE]->image->Picture->LoadFromFile("bmp\\icon_line.bmp");
    icon[HOOK]->image->Picture->LoadFromFile("bmp\\icon_hook.bmp");
    icon[INTERCOM]->image->Picture->LoadFromFile("bmp\\icon_intercom.bmp");
    icon[MUTE]->image->Picture->LoadFromFile("bmp\\icon_mute.bmp");
    if (aCfg->batteryStatus == "half") {
        icon[BATTERY]->image->Picture->LoadFromFile("bmp\\icon_battery_half.bmp");
    }
    else if (aCfg->batteryStatus == "low") {
        icon[BATTERY]->image->Picture->LoadFromFile("bmp\\icon_battery_low.bmp");
    }
    else {
        icon[BATTERY]->image->Picture->LoadFromFile("bmp\\icon_battery_full.bmp");
    }
    icon[ANTENNA]->image->Picture->LoadFromFile("bmp\\icon_antenna.bmp");
    icon[SPEAKER]->image->Picture->LoadFromFile("bmp\\icon_speaker.bmp");
    icon[KMESSAGE]->image->Picture->LoadFromFile("bmp\\icon_message.bmp");

    icon[BELL] = new Icon;
    icon[BELL]->image->Left = line[1]->pos[0]->ch->Left + ICON_WIDTH;
    icon[BELL]->image->Top = line[1]->pos[0]->ch->Top;
    icon[BELL]->image->Width = ICON_WIDTH;
    icon[BELL]->image->Height = line[1]->pos[0]->ch->Height;
    icon[BELL]->image->Transparent = true;
    icon[BELL]->image->Stretch = true;
    icon[BELL]->image->Visible = false;
    icon[BELL]->image->Picture->LoadFromFile("bmp\\icon_bell.bmp");

    icon[SELECT1] = new Icon;
    icon[SELECT1]->image->Left = line[0]->pos[14]->ch->Left+3;
    icon[SELECT1]->image->Top = line[0]->pos[14]->ch->Top+1;
    icon[SELECT1]->image->Width = line[0]->pos[15]->ch->Height-2;
    icon[SELECT1]->image->Height = line[0]->pos[15]->ch->Height-2;
    icon[SELECT1]->image->Transparent = true;
    icon[SELECT1]->image->Stretch = true;
    icon[SELECT1]->image->Visible = false;
    icon[SELECT1]->image->Picture->LoadFromFile("bmp\\icon_select.bmp");

    icon[SELECT2] = new Icon;
    icon[SELECT2]->image->Left = line[1]->pos[14]->ch->Left+3;
    icon[SELECT2]->image->Top = line[1]->pos[15]->ch->Top+1;
    icon[SELECT2]->image->Width = line[1]->pos[15]->ch->Height-2;
    icon[SELECT2]->image->Height = line[1]->pos[15]->ch->Height-2;
    icon[SELECT2]->image->Transparent = true;
    icon[SELECT2]->image->Stretch = true;
    icon[SELECT2]->image->Visible = false;
    icon[SELECT2]->image->Picture->LoadFromFile("bmp\\icon_select.bmp");
};

// gamma:need to change when porting a new handset image.
KeyPad::KeyPad() { // set rectangle for each keypad to display
/* The value about position and size of keys are measured in the original bitmap image.
   So, we need to adust according the size of Image1.*/
    AnsiString dir = "bmp\\";

    key[KEY_UP].Left = 90;
    key[KEY_UP].Top = 234;
    key[KEY_UP].Width = 90;
    key[KEY_UP].Height = 64;
    key[KEY_UP].color = clRed;
    key[KEY_UP].bitMapFile = dir + "key_up.bmp";

    key[KEY_DOWN].Left = 92;
    key[KEY_DOWN].Top = 232;
    key[KEY_DOWN].Width = 88;
    key[KEY_DOWN].Height = 63;
    key[KEY_DOWN].color = TColor(clRed - 1);
    key[KEY_DOWN].bitMapFile = dir + "key_down.bmp";

    key[KEY_LEFT].Left = 91;
    key[KEY_LEFT].Top = 232;
    key[KEY_LEFT].Width = 90;
    key[KEY_LEFT].Height = 64;
    key[KEY_LEFT].color = TColor(clRed - 2);
    key[KEY_LEFT].bitMapFile = dir + "key_left.bmp";

    key[KEY_RIGHT].Left = 89;
    key[KEY_RIGHT].Top = 231;
    key[KEY_RIGHT].Width = 89;
    key[KEY_RIGHT].Height = 66;
    key[KEY_RIGHT].color = TColor(clRed - 3);
    key[KEY_RIGHT].bitMapFile = dir + "key_right.bmp";

    key[KEY_REDIAL].Left = 40;
    key[KEY_REDIAL].Top = 233;
    key[KEY_REDIAL].Width = 53;
    key[KEY_REDIAL].Height = 30;
    key[KEY_REDIAL].color = TColor(clRed - 4);
    key[KEY_REDIAL].bitMapFile = dir + "key_redial.bmp";

    key[KEY_ON_OFF].Left = 38;
    key[KEY_ON_OFF].Top = 264;
    key[KEY_ON_OFF].Width = 59;
    key[KEY_ON_OFF].Height = 35;
    key[KEY_ON_OFF].color = TColor(clRed - 5);
    key[KEY_ON_OFF].bitMapFile = dir + "key_on_off.bmp";
         
    key[KEY_TOP_MESSAGE].Left = 44;
    key[KEY_TOP_MESSAGE].Top = 289;
    key[KEY_TOP_MESSAGE].Width = 68;
    key[KEY_TOP_MESSAGE].Height = 34;
    key[KEY_TOP_MESSAGE].color = TColor(clRed - 6);
    key[KEY_TOP_MESSAGE].bitMapFile = dir + "key_top_message.bmp";
                       
    key[KEY_SOS_SMS].Left = 100;
    key[KEY_SOS_SMS].Top = 295;
    key[KEY_SOS_SMS].Width = 68;
    key[KEY_SOS_SMS].Height = 30;
    key[KEY_SOS_SMS].color = TColor(clRed - 7);
    key[KEY_SOS_SMS].bitMapFile = dir + "key_sos_sms.bmp";

    key[KEY_SMS].Left = 158;
    key[KEY_SMS].Top = 291;
    key[KEY_SMS].Width = 68;
    key[KEY_SMS].Height = 30;
    key[KEY_SMS].color = TColor(clRed - 8);
    key[KEY_SMS].bitMapFile = dir + "key_sms.bmp";

    key[KEY_HANDSFREE].Left = 177;
    key[KEY_HANDSFREE].Top = 266;
    key[KEY_HANDSFREE].Width = 51;
    key[KEY_HANDSFREE].Height = 29;
    key[KEY_HANDSFREE].color = TColor(clRed - 9);
    key[KEY_HANDSFREE].bitMapFile = dir + "key_handsfree.bmp";

    key[KEY_MENU].Left = 174;
    key[KEY_MENU].Top = 232;
    key[KEY_MENU].Width = 53;
    key[KEY_MENU].Height = 29;
    key[KEY_MENU].color = TColor(clRed - 10);
    key[KEY_MENU].bitMapFile = dir + "key_menu.bmp";

    key[KEY_1].Left = 39;
    key[KEY_1].Top = 326;
    key[KEY_1].Width = 55;
    key[KEY_1].Height = 28;
    key[KEY_1].color = TColor(clRed - 11);
    key[KEY_1].bitMapFile = dir + "key_1.bmp";
    strcpy(key[KEY_1].symbol, "1");

    key[KEY_2].Left = 102;
    key[KEY_2].Top = 330;
    key[KEY_2].Width = 64;
    key[KEY_2].Height = 27;
    key[KEY_2].color = TColor(clRed - 12);
    key[KEY_2].bitMapFile = dir + "key_2.bmp";
    strcpy(key[KEY_2].symbol, "2");

    key[KEY_3].Left = 172;
    key[KEY_3].Top = 327;
    key[KEY_3].Width = 57;
    key[KEY_3].Height = 27;
    key[KEY_3].color = TColor(clRed - 13);
    key[KEY_3].bitMapFile = dir + "key_3.bmp";
    strcpy(key[KEY_3].symbol, "3");

    key[KEY_4].Left = 41;
    key[KEY_4].Top = 358;
    key[KEY_4].Width = 55;
    key[KEY_4].Height = 27;
    key[KEY_4].color = TColor(clRed - 14);
    key[KEY_4].bitMapFile = dir + "key_4.bmp";
    strcpy(key[KEY_4].symbol, "4");

    key[KEY_5].Left = 102;
    key[KEY_5].Top = 361;
    key[KEY_5].Width = 61;
    key[KEY_5].Height = 26;
    key[KEY_5].color = TColor(clRed - 15);
    key[KEY_5].bitMapFile = dir + "key_5.bmp";
    strcpy(key[KEY_5].symbol, "5");

    key[KEY_6].Left = 170;
    key[KEY_6].Top = 359;
    key[KEY_6].Width = 57;
    key[KEY_6].Height = 26;
    key[KEY_6].color = TColor(clRed - 16);
    key[KEY_6].bitMapFile = dir + "key_6.bmp";
    strcpy(key[KEY_6].symbol, "6");

    key[KEY_7].Left = 44;
    key[KEY_7].Top = 389;
    key[KEY_7].Width = 54;
    key[KEY_7].Height = 26;
    key[KEY_7].color = TColor(clRed - 17);
    key[KEY_7].bitMapFile = dir + "key_7.bmp";
    strcpy(key[KEY_7].symbol, "7");

    key[KEY_8].Left = 104;
    key[KEY_8].Top = 391;
    key[KEY_8].Width = 59;
    key[KEY_8].Height = 25;
    key[KEY_8].color = TColor(clRed - 18);
    key[KEY_8].bitMapFile = dir + "key_8.bmp";
    strcpy(key[KEY_8].symbol, "8");

    key[KEY_9].Left = 170;
    key[KEY_9].Top = 389;
    key[KEY_9].Width = 54;
    key[KEY_9].Height = 26;
    key[KEY_9].color = TColor(clRed - 19);
    key[KEY_9].bitMapFile = dir + "key_9.bmp";
    strcpy(key[KEY_9].symbol, "9");

    key[KEY_STAR].Left = 51;
    key[KEY_STAR].Top = 420;
    key[KEY_STAR].Width = 49;
    key[KEY_STAR].Height = 26;
    key[KEY_STAR].color = TColor(clRed - 20);
    key[KEY_STAR].bitMapFile = dir + "key_star.bmp";
    strcpy(key[KEY_STAR].symbol, "*");

    key[KEY_0].Left = 106;
    key[KEY_0].Top = 422;
    key[KEY_0].Width = 56;
    key[KEY_0].Height = 24;
    key[KEY_0].color = TColor(clRed - 21);
    key[KEY_0].bitMapFile = dir + "key_0.bmp";
    strcpy(key[KEY_0].symbol, "0");

    key[KEY_HASH].Left = 169;
    key[KEY_HASH].Top = 420;
    key[KEY_HASH].Width = 50;
    key[KEY_HASH].Height = 26;
    key[KEY_HASH].color = TColor(clRed - 22);
    key[KEY_HASH].bitMapFile = dir + "key_hash.bmp";
    strcpy(key[KEY_HASH].symbol, "#");

    key[KEY_FT_SERVICES].Left = 68;
    key[KEY_FT_SERVICES].Top = 458;
    key[KEY_FT_SERVICES].Width = 34;
    key[KEY_FT_SERVICES].Height = 27;
    key[KEY_FT_SERVICES].color = TColor(clRed - 23);
    key[KEY_FT_SERVICES].bitMapFile = dir + "key_ft_services.bmp";

    key[KEY_SOS].Left = 166;
    key[KEY_SOS].Top = 458;
    key[KEY_SOS].Width = 35;
    key[KEY_SOS].Height = 28;
    key[KEY_SOS].color = TColor(clRed - 24);
    key[KEY_SOS].bitMapFile = dir + "key_sos.bmp";



    for (int i = KEY_UP; i < KEY_SIZE; i++) { // bitmap size is (270, 530)
        key[i].Left = key[i].Left * Form1->Image1->Width / 270;
        key[i].Top = key[i].Top * Form1->Image1->Height / 530;
        key[i].Width = key[i].Width * Form1->Image1->Width / 270;
        key[i].Height = key[i].Height * Form1->Image1->Height / 530;
    }
    return;
};

/*
Return the key_id of the key be pressed. Return -1 if (x, y) is not on any key.
Image3 painting each key button with different color.
So, we decide Mouse Down (x, y) is to press a specified key by color of
pixel(x, y) in Image3 is the same with Key Color.
*/
int KeyPad::WhichKeyPressed(int x, int y) {
    int i;

    int x1 = x*270/Form1->Image3->Width; // adjust (x, y) by the ratio of original image to (Image3->Width, Image3->Height)
    int y1 = y*530/Form1->Image3->Height;

    for (i = KEY_UP; i <= KEY_SMS; i++) {
        if (Form1->Image3->Picture->Bitmap->Canvas->Pixels[x1][y1] == key[i].color) {
            return i;
        }
    }
    return -1;
}

PT::PT() {
    cfg = new PTConfig;
    lcd = new Lcd(cfg);
    kp = new KeyPad;
    buzzar = new Buzzar;
    menuTree = new MenuTree();
    sms = new SMS;
    mmi = new PTMMI;
            
    Form1->TreeView1->Visible = false;

    Form1->Image1->Top = 0;
    Form1->Image1->Left = 0;

    Form1->Image2->Visible = false;

    Form1->Image3->Left = Form1->Image1->Left;
    Form1->Image3->Top = Form1->Image1->Top;
    Form1->Image3->Width = Form1->Image1->Width;
    Form1->Image3->Height = Form1->Image1->Height;
    Form1->Image3->Picture->LoadFromFile("bmp\\HS_red.bmp"); // gamma:need to change when porting.
    Form1->Image3->Visible = false;
};