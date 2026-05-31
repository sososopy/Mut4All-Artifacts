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

pub struct InherentAssocTypeSubtypingConflict_421;

impl Mutator for InherentAssocTypeSubtypingConflict_421 {
    fn name(&self) -> &str {
        "InherentAssocTypeSubtypingConflict_421"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AssocTypeInjector {};
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct AssocTypeInjector {
    foo_inserted: bool,
}

impl AssocTypeInjector {
    fn new() -> Self {
        Self { foo_inserted: false }
    }
}

impl VisitMut for AssocTypeInjector {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        for input in &mut i.sig.inputs {
            if let FnArg::Typed(pat_type) = input {
                if let Type::BareFn(bare_fn) = &*pat_type.ty {
                    if bare_fn.abi.is_default() && !bare_fn.inputs.is_empty() {
                        let inner_type = &bare_fn.inputs[0];
                        let assoc_type = parse_quote!(Foo<#inner_type>::Assoc);
                        pat_type.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: parse_quote!(Foo<#inner_type>::Assoc),
                        }));
                    }
                }
            }
        }
    }

    fn visit_item_struct_mut(&mut self, _: &mut ItemStruct) {
        if !self.foo_inserted {
            self.foo_inserted = true;
            let foo_def = parse_quote! {
                struct Foo<T>(T);
                impl<'a, F> Foo<for<'a> fn(&'a ())> {
                    type Assoc = &'a ();
                }
            };
            if let Some(first_item) = file.items.first_mut() {
                file.items.insert(0, Item::Struct(first_item.take().into()));
                file.items.insert(1, foo_def);
            } else {
                file.items.push(foo_def);
            }
        }
    }
}