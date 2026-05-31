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

pub struct Replace_NoMangle_With_InlineNever_And_Wrapper_119;

impl Mutator for Replace_NoMangle_With_InlineNever_And_Wrapper_119 {
    fn name(&self) -> &str {
        "Replace_NoMangle_With_InlineNever_And_Wrapper_119"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_no_mangle = false;
                let mut attrs_to_keep = Vec::new();
                for attr in &item_fn.attrs {
                    if attr.path().is_ident("no_mangle") {
                        has_no_mangle = true;
                    } else {
                        attrs_to_keep.push(attr.clone());
                    }
                }
                if has_no_mangle {
                    attrs_to_keep.push(parse_quote!(#[inline(never)]));
                    item_fn.attrs = attrs_to_keep;
                    let wrapper_name = Ident::new(&format!("{}_wrapper", item_fn.sig.ident), Span::call_site());
                    let mut wrapper_fn = ItemFn {
                        attrs: vec![parse_quote!(#[no_mangle])],
                        vis: item_fn.vis.clone(),
                        sig: item_fn.sig.clone(),
                        block: Box::new(syn::Block {
                            brace_token: token::Brace::default(),
                            stmts: vec![Stmt::Expr(
                                Expr::Call(ExprCall {
                                    attrs: Vec::new(),
                                    func: Box::new(Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(item_fn.sig.ident.clone()),
                                    })),
                                    paren_token: token::Paren::default(),
                                    args: Punctuated::new(),
                                }),
                                None,
                            )],
                        }),
                    };
                    wrapper_fn.sig.ident = wrapper_name;
                    new_items.push(Item::Fn(wrapper_fn));
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions annotated with #[no_mangle]. It removes the #[no_mangle] attribute from the original function and adds #[inline(never)] to it. Then, it creates a new wrapper function with the same signature but a name derived by appending '_wrapper' to the original identifier, annotated with #[no_mangle]. The wrapper function calls the original function. This transformation changes the linkage context around monomorphization points, potentially exposing bugs related to code generation, symbol naming, and interactions with async blocks, generics, or panic=abort settings."
    }
}