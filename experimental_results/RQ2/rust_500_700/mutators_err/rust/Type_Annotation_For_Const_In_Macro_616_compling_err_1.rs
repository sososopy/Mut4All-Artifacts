use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Macro, Pat, PatType, Path as SynPath,
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

pub struct Type_Annotation_For_Const_In_Macro_616;

impl Mutator for Type_Annotation_For_Const_In_Macro_616 {
    fn name(&self) -> &str {
        "Type_Annotation_For_Const_In_Macro_616"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if let Some((_, mac_content)) = mac.mac.tokens.clone().into_iter().next() {
                    if let Ok(parsed) = syn::parse2::<Macro>(mac_content) {
                        let mut new_tokens = TokenStream::new();
                        for token in parsed.mac.tokens {
                            if let Ok(mut const_item) = syn::parse2::<ItemConst>(token.clone()) {
                                if const_item.ty.is_none() {
                                    const_item.ty = Some(Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("i32", Span::call_site())),
                                    })));
                                    const_item.expr = Some(Box::new(Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: SynPath::from(Ident::new("0", Span::call_site())),
                                    })));
                                }
                                const_item.to_tokens(&mut new_tokens);
                            } else {
                                token.to_tokens(&mut new_tokens);
                            }
                        }
                        mac.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const items in macro definitions that lack type annotations and adds an explicit type annotation. This transformation aims to expose issues in type inference and macro expansion by enforcing explicit type checking."
    }
}