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
        let mut visitor = AssocTypeInjector::new(file);
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct AssocTypeInjector<'a> {
    foo_inserted: bool,
    file: &'a mut File,
}

impl<'a> AssocTypeInjector<'a> {
    fn new(file: &'a mut File) -> Self {
        Self { foo_inserted: false, file }
    }
}

impl<'a> VisitMut for AssocTypeInjector<'a> {
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

    fn visit_item_struct_mut(&mut self, _: &mut ItemStruct) {
        if !self.foo_inserted {
            self.foo_inserted = true;
            let foo_def = parse_quote! {
                struct Foo<T>(T);
                impl<'a, F> Foo<for<'a> fn(&'a ())> {
                    type Assoc = &'a ();
                }
            };
            if let Some(first_item) = self.file.items.pop(0) {
                self.file.items.insert(0, foo_def);
                self.file.items.insert(1, first_item);
            } else {
                self.file.items.push(foo_def);
            }
        }
    }
}