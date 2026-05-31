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

pub struct Add_Complex_ThreadLocal_Initialization_210;

impl Mutator for Add_Complex_ThreadLocal_Initialization_210 {
    fn name(&self) -> &str {
        "Add_Complex_ThreadLocal_Initialization_210"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("thread_local") {
                    if let Ok(content) = item_macro.mac.parse_body::<TokenStream>() {
                        let mut new_content = TokenStream::new();
                        let mut replaced = false;
                        for token in content {
                            if let proc_macro2::TokenTree::Ident(ident) = &token {
                                if ident == "static" && !replaced {
                                    replaced = true;
                                    new_content.extend(quote! {
                                        static FOO: Type = {
                                            let x = Type::new();
                                            if condition() {
                                                x.modify();
                                            }
                                            x
                                        }
                                    });
                                } else {
                                    new_content.extend(quote! {#token});
                                }
                            } else {
                                new_content.extend(quote! {#token});
                            }
                        }
                        item_macro.mac.tokens = new_content;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets thread-local storage macros by modifying the initialization of a thread-local variable to include a conditional block. It introduces complexity in the initialization logic, aiming to stress-test the compiler's handling of thread-local variables with conditional expressions, particularly within macro-expanded code."
    }
}