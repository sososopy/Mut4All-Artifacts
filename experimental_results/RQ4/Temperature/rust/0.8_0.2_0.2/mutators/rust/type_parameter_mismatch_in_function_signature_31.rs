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

pub struct Type_Parameter_Mismatch_In_Function_Signature_31;

impl Mutator for Type_Parameter_Mismatch_In_Function_Signature_31 {
    fn name(&self) -> &str {
        "Type_Parameter_Mismatch_In_Function_Signature_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &mut **ty {
                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut last_segment.arguments {
                                let mut rng = thread_rng();
                                let mut type_params: Vec<_> = angle_bracketed.args.iter().cloned().collect();
                                if type_params.len() > 1 {
                                    type_params.shuffle(&mut rng);
                                    angle_bracketed.args.clear();
                                    angle_bracketed.args.extend(type_params);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with generic type parameters in their return type. It deliberately mismatches the order of these type parameters to introduce subtle inconsistencies. This transformation tests the compiler's ability to handle type parameter mismatches and may reveal hidden bugs in the type system handling logic."
    }
}