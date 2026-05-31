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

pub struct Introduce_Nested_Local_Struct_Derivation_255;

impl Mutator for Introduce_Nested_Local_Struct_Derivation_255 {
    fn name(&self) -> &str {
        "Introduce_Nested_Local_Struct_Derivation_255"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(item_fn) = impl_item {
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Item(Item::Struct(item_struct)) = stmt {
                                if item_struct
                                    .attrs
                                    .iter()
                                    .any(|attr| attr.path.is_ident("derive"))
                                {
                                    let nested_struct: ItemStruct = parse_quote! {
                                        #[derive(serde::Deserialize)]
                                        struct Nested {}
                                    };
                                    let nested_struct_tokens = quote! { #nested_struct };
                                    let nested_struct_item: syn::Item =
                                        syn::parse2(nested_struct_tokens).unwrap();
                                    let nested_stmt = Stmt::Item(nested_struct_item);
                                    if let Some(last_brace) =
                                        item_struct.brace_token.span.join().map(|span| span.end())
                                    {
                                        let nested_stmt_tokens = quote! { #nested_stmt };
                                        let nested_stmt_tokens_with_braces = quote_spanned! {
                                            last_brace => #nested_stmt_tokens
                                        };
                                        *stmt = Stmt::Verbatim(nested_stmt_tokens_with_braces);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets local struct definitions within impl blocks that use procedural macro derives, such as `serde::Deserialize`. It introduces a nested struct within the existing struct, applying the same derive attribute. This transformation increases the complexity of the derive macro evaluation, potentially leading to stack overflow or other macro expansion issues, thereby testing the compiler's robustness in handling deeply nested procedural macro invocations."
    }
}