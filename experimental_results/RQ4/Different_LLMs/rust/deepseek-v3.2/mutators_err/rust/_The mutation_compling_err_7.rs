use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Macro_Body_With_Box_Keyword_236;

impl Mutator for Replace_Macro_Body_With_Box_Keyword_236 {
    fn name(&self) -> &str {
        "Replace_Macro_Body_With_Box_Keyword_236"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(mac) = &item_macro.mac {
                    if mac.path.is_ident("macro_rules") {
                        let mut rng = thread_rng();
                        let mut visitor = MacroRulesVisitor {
                            rng: &mut rng,
                            mutated: false,
                        };
                        visitor.visit_macro_mut(mac);
                        if visitor.mutated {
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct MacroRulesVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    mutated: bool,
}

impl<'a> VisitMut for MacroRulesVisitor<'a> {
    fn visit_macro_mut(&mut self, mac: &mut syn::Macro) {
        if let Some(token_stream) = &mut mac.tokens {
            let mut tokens = token_stream.clone().into_iter().collect::<Vec<_>>();
            if tokens.len() > 0 {
                let index = self.rng.gen_range(0..tokens.len());
                tokens[index] = proc_macro2::TokenTree::Ident(proc_macro2::Ident::new("box", Span::call_site()));
                mac.tokens = Some(proc_macro2::TokenStream::from_iter(tokens));
                self.mutated = true;
            }
        }
    }
}