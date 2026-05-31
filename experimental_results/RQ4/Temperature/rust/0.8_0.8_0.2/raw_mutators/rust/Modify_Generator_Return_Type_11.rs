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

pub struct Modify_Generator_Return_Type_11;

impl Mutator for Modify_Generator_Return_Type_11 {
    fn name(&self) -> &str {
        "Modify_Generator_Return_Type_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let new_return_type: Type = parse_quote! { Option<()> };
                        **return_type = new_return_type;

                        let return_expr: Expr = parse_quote! { Some(()) };
                        if let Some(last_stmt) = item_fn.block.stmts.last_mut() {
                            if let Stmt::Expr(expr, _) = last_stmt {
                                *expr = parse_quote! {
                                    { yield (); return #return_expr; }
                                };
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator changes the return type of generator functions from `()` to `Option<()>`, potentially uncovering issues related to type alias implementation traits in generators, especially when using `impl Trait`. It modifies both the return type and the generator's logic to ensure compatibility, aiming to expose latent bugs when the return type is altered."
    }
}