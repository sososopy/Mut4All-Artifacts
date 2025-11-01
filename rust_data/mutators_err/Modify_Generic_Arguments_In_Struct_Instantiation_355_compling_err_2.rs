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

pub struct Modify_Generic_Arguments_In_Struct_Instantiation_355;

impl Mutator for Modify_Generic_Arguments_In_Struct_Instantiation_355 {
    fn name(&self) -> &str {
        "Modify_Generic_Arguments_In_Struct_Instantiation_355"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        struct StructInstantiationVisitor<'a> {
            types: &'a HashSet<String>,
        }

        impl<'a> VisitMut for StructInstantiationVisitor<'a> {
            fn visit_expr_path_mut(&mut self, i: &mut ExprPath) {
                if let Some(last_segment) = i.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                        let existing_types: Vec<&GenericArgument> = args.args.iter().collect();
                        if !existing_types.is_empty() {
                            let new_type: GenericArgument = parse_quote!(i32);
                            args.args.push_value(new_type);
                            args.args.push_punct(Comma::default());
                        }
                    }
                }
                visit_mut::visit_expr_path_mut(self, i);
            }
        }

        let mut types = HashSet::new();
        for item in &file.items {
            if let Item::Struct(ItemStruct { ident, .. }) = item {
                types.insert(ident.to_string());
            }
        }

        let mut visitor = StructInstantiationVisitor { types: &types };
        visitor.visit_file_mut(file);
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct instantiations with generic arguments. It adds an extra generic argument to the instantiation, using a simple type such as i32. This can cause a mismatch in the expected number of generic parameters, potentially triggering compiler errors or unexpected behavior."
    }
}