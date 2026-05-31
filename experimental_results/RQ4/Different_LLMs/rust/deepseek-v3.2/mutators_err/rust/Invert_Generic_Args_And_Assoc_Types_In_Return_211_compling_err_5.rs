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

pub struct Invert_Generic_Args_And_Assoc_Types_In_Return_211;

impl Mutator for Invert_Generic_Args_And_Assoc_Types_In_Return_211 {
    fn name(&self) -> &str {
        "Invert_Generic_Args_And_Assoc_Types_In_Return_211"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ReturnTypeVisitor<'a> {
            mutator: &'a Invert_Generic_Args_And_Assoc_Types_In_Return_211,
        }
        impl<'a> VisitMut for ReturnTypeVisitor<'a> {
            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                self.mutator.mutate_return_type(&mut i.sig.output);
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
            fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
                self.mutator.mutate_return_type(&mut i.sig.output);
                syn::visit_mut::visit_impl_item_fn_mut(self, i);
            }
            fn visit_trait_item_fn_mut(&mut self, i: &mut syn::TraitItemFn) {
                if let syn::TraitItemFn {
                    sig,
                    default: Some(_),
                    ..
                } = i
                {
                    self.mutator.mutate_return_type(&mut sig.output);
                }
                syn::visit_mut::visit_trait_item_fn_mut(self, i);
            }
        }
        let mut visitor = ReturnTypeVisitor { mutator: self };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inverts the order of generic arguments and associated type bindings in method return types. It targets return type annotations that are generic paths containing both regular generic arguments and associated type bindings separated by commas. The operator swaps their positions, e.g., `Path<GenericArg, Item = Type>` becomes `Path<Item = Type, GenericArg>`. This reordering is syntactically valid but may expose compiler bugs in metadata decoding or associated item resolution, as seen in bug reports where invalid generic argument order triggered ICEs."
    }
}
impl Invert_Generic_Args_And_Assoc_Types_In_Return_211 {
    fn mutate_return_type(&self, return_type: &mut ReturnType) {
        if let ReturnType::Type(_, boxed_type) = return_type {
            if let Type::Path(TypePath { path, .. }) = &mut **boxed_type {
                if let Some(last_segment) = path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        let mut generic_args = Punctuated::<GenericArgument, Comma>::new();
                        let mut assoc_bindings = Punctuated::<GenericArgument, Comma>::new();
                        let mut has_generic_arg = false;
                        let mut has_assoc_binding = false;
                        for arg in args.args.iter() {
                            match arg {
                                GenericArgument::Type(_)
                                | GenericArgument::Lifetime(_)
                                | GenericArgument::Const(_)
                                | GenericArgument::AssocType(_) => {
                                    if let GenericArgument::AssocType(_) = arg {
                                        has_assoc_binding = true;
                                        assoc_bindings.push(arg.clone());
                                    } else {
                                        has_generic_arg = true;
                                        generic_args.push(arg.clone());
                                    }
                                }
                                _ => {}
                            }
                        }
                        if has_generic_arg && has_assoc_binding && !assoc_bindings.is_empty() {
                            let mut new_args = Punctuated::new();
                            new_args.extend(assoc_bindings);
                            new_args.extend(generic_args);
                            args.args = new_args;
                        }
                    }
                }
            }
        }
    }
}