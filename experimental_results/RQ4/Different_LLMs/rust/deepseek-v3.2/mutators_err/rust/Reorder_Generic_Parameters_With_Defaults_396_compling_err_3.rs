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

pub struct Reorder_Generic_Parameters_With_Defaults_396;

impl Mutator for Reorder_Generic_Parameters_With_Defaults_396 {
    fn name(&self) -> &str {
        "Reorder_Generic_Parameters_With_Defaults_396"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReorderVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator reorders generic parameters in struct, enum, trait, impl, and function definitions when there are at least two generic parameters and at least one const generic with a default expression. It swaps the positions of two consecutive generic parameters, placing a const generic with a default before a type parameter without a default (or vice versa). This reordering may expose mismatches in the compiler's internal indexing or substitution logic for generic arguments, potentially triggering ICEs during compilation."
    }
}

struct ReorderVisitor;

impl VisitMut for ReorderVisitor {
    fn visit_item_mut(&mut self, item: &mut Item) {
        match item {
            Item::Struct(item_struct) => {
                self.reorder_generics(&mut item_struct.generics);
            }
            Item::Enum(item_enum) => {
                self.reorder_generics(&mut item_enum.generics);
            }
            Item::Trait(item_trait) => {
                self.reorder_generics(&mut item_trait.generics);
            }
            Item::Impl(item_impl) => {
                self.reorder_generics(&mut item_impl.generics);
            }
            Item::Fn(item_fn) => {
                self.reorder_generics(&mut item_fn.sig.generics);
            }
            _ => {}
        }
        syn::visit_mut::visit_item_mut(self, item);
    }
}

impl ReorderVisitor {
    fn reorder_generics(&self, generics: &mut syn::Generics) {
        if generics.params.len() < 2 {
            return;
        }

        let mut rng = thread_rng();
        let mut indices: Vec<usize> = (0..generics.params.len() - 1).collect();
        indices.shuffle(&mut rng);

        for i in indices {
            let current = &generics.params[i];
            let next = &generics.params[i + 1];

            let current_is_const_with_default = matches!(current, GenericParam::Const(c) if c.default.is_some());
            let next_is_const_with_default = matches!(next, GenericParam::Const(c) if c.default.is_some());
            let current_is_type_without_default = matches!(current, GenericParam::Type(t) if t.default.is_none());
            let next_is_type_without_default = matches!(next, GenericParam::Type(t) if t.default.is_none());

            let should_swap = (current_is_const_with_default && next_is_type_without_default)
                || (current_is_type_without_default && next_is_const_with_default);

            if should_swap {
                let mut params_vec: Vec<GenericParam> = generics.params.iter().cloned().collect();
                params_vec.swap(i, i + 1);
                generics.params = params_vec.into_iter().collect();
                break;
            }
        }
    }
}