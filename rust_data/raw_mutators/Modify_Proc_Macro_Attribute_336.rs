use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Proc_Macro_Attribute_336;

impl Mutator for Modify_Proc_Macro_Attribute_336 {
    fn name(&self) -> &str {
        "Modify_Proc_Macro_Attribute_336"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_identity_function = false;

        for item in &mut file.items {
            if let Item::Fn(ref mut item_fn) = item {
                if item_fn.attrs.iter().any(|attr| attr.path.is_ident("proc_macro_attribute")) {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Semi(Expr::Macro(ref mut expr_macro), _) = stmt {
                            if expr_macro.mac.path.is_ident("panic") {
                                if let Some(existing_fn) = find_existing_function(file) {
                                    expr_macro.mac.path = existing_fn;
                                } else {
                                    expr_macro.mac.path = parse_quote!(identity_function);
                                    has_identity_function = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if has_identity_function {
            file.items.push(parse_quote! {
                fn identity_function(input: TokenStream) -> TokenStream {
                    input
                }
            });
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions annotated with #[proc_macro_attribute]. It replaces 'panic!()' calls with a call to an existing function that returns a TokenStream, or introduces an identity function if no suitable function exists. This change modifies the macro's behavior from immediate panic to a transformation of the input."
    }
}

fn find_existing_function(file: &syn::File) -> Option<SynPath> {
    for item in &file.items {
        if let Item::Fn(ref item_fn) = item {
            if let ReturnType::Type(_, ref ty) = item_fn.sig.output {
                if let Type::Path(TypePath { path, .. }) = &**ty {
                    if path.is_ident("TokenStream") {
                        return Some(item_fn.sig.ident.clone().into());
                    }
                }
            }
        }
    }
    None
}