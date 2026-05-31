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

pub struct Replace_TAIT_With_Concrete_Drop_Struct_479;

impl Mutator for Replace_TAIT_With_Concrete_Drop_Struct_479 {
    fn name(&self) -> &str {
        "Replace_TAIT_With_Concrete_Drop_Struct_479"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tait_aliases = Vec::new();
        let mut candidate_structs = Vec::new();
        
        for item in &file.items {
            if let Item::Type(alias) = item {
                if let Type::ImplTrait(_) = &*alias.ty {
                    tait_aliases.push(alias.ident.clone());
                }
            }
            if let Item::Struct(strct) = item {
                let mut has_drop_glue = false;
                if let syn::Fields::Named(fields) = &strct.fields {
                    for field in &fields.named {
                        if let Type::Path(type_path) = &field.ty {
                            let type_name = type_path.path.segments.last().map(|seg| seg.ident.to_string());
                            if let Some(name) = type_name {
                                if matches!(name.as_str(), "String" | "Vec" | "Box") {
                                    has_drop_glue = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_drop_glue {
                    candidate_structs.push(strct.ident.clone());
                }
            }
        }
        
        if tait_aliases.is_empty() || candidate_structs.is_empty() {
            return;
        }
        
        let mut rng = thread_rng();
        let target_tait = tait_aliases.choose(&mut rng).unwrap();
        let target_struct = candidate_structs.choose(&mut rng).unwrap();
        
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.len() ==,1 && type_path.path.segments[0].ident == *target_tait {
                                pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(target_struct.clone()),
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type alias impl trait (TAIT) usage in function parameters with a concrete struct type that has nontrivial drop glue. It first collects all TAIT aliases and structs containing fields with drop-glue types (String, Vec, Box). Then randomly selects one TAIT and one candidate struct, replacing all function parameter uses of that TAIT with the selected struct type. This transformation tests the compiler's drop elaboration when TAITs are replaced by concrete types with destructors, potentially exposing bugs in destructuring operations."
    }
}