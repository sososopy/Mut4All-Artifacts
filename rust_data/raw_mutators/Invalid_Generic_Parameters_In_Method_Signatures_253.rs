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

pub struct Invalid_Generic_Parameters_In_Method_Signatures_253;

impl Mutator for Invalid_Generic_Parameters_In_Method_Signatures_253 {
    fn name(&self) -> &str {
        "Invalid_Generic_Parameters_In_Method_Signatures_253"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MethodVisitor;

        impl VisitMut for MethodVisitor {
            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                if let ReturnType::Type(_, ref mut ty) = i.sig.output {
                    if let Type::Path(TypePath { path: ref mut p, .. }) = **ty {
                        if p.segments.last().map_or(false, |seg| seg.ident.to_string().contains("Iter")) {
                            if let Some(last_segment) = p.segments.last_mut() {
                                if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Type(ref mut typ) = arg {
                                            *typ = parse_quote! { NonExistentType };
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
        }

        let mut visitor = MethodVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets methods within a struct or impl block that return iterator types. It modifies the generic parameters of these iterator types by replacing them with invalid or non-existent types. This mutation can potentially cause the Rust compiler to exhibit unexpected behavior due to the invalid type parameters."
    }
}