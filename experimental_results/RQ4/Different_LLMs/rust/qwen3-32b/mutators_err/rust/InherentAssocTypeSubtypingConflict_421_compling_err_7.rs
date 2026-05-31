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
        let mut visitor = AssocTypeInjector::new();
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
                    if bare_fn.abi.is_none() && !bare_fn.inputs.is_empty() {
                        let inner_type = &bare_fn.inputs[0];
                        let assoc_type: Type = parse_quote!(Foo<#inner_type>::Assoc);
                        pat_type.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: parse_quote!(Foo<#inner_type>::Assoc),
                        }));
                    }
                }
            }
        }
    }

    fn visit_file_mut(&mut self, file: &mut File) {
        if !self.foo_inserted {
            let mut first_struct_idx = None;
            for (i, item) in file.items.iter().enumerate() {
                if let Item::Struct(_) = item {
                    first_struct_idx = Some(i);
                    break;
                }
            }

            if let Some(idx) = first_struct_idx {
                let foo_def = parse_quote! {
                    struct Foo<T>(T);
                    impl<'a, F> Foo<for<'b> fn(&'b ())> {
                        type Assoc = &'a ();
                    }
                };
                if idx > 0 {
                    let first_item = file.items.remove(idx);
                    file.items.insert(idx, foo_def);
                    file.items.insert(idx + 1, first_item);
                } else {
                    file.items.insert(0, foo_def);
                }
                self.foo_inserted = true;
            }
        }
        syn::visit_mut::visit_file_mut(self, file);
    }
}