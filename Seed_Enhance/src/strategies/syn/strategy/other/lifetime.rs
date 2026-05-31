use proc_macro2::Span;
use syn::{Label, Lifetime};

use crate::{
    gen_token,
    strategies::syn::strategy::consts::{LIFETIME_B, LIFETIME_R},
    util::{gen_alpha, glob_range},
};

pub fn gen_lifetime() -> Option<Lifetime> {
    if LIFETIME_B < glob_range(0. ..1.) {
        return None;
    }
    let len = glob_range(LIFETIME_R);
    let ident = gen_alpha(len);
    let ident = "\'".to_owned() + &ident;
    Some(Lifetime::new(&ident, Span::call_site()))
}

pub fn gen_label() -> Option<Label> {
    gen_lifetime().map(|x| Label {
        name: x,
        colon_token: gen_token!(:),
    })
}
