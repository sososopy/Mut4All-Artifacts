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

pub struct Modify_Const_Generic_Parameters_385;

impl Mutator for Modify_Const_Generic_Parameters_385 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_385"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_generic_indices = Vec::new();
                for (i, param) in item_struct.generics.params.iter().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if !matches!(
                            &const_param.ty,
                            Type::Path(TypePath { path, .. })
                                if path.is_ident("usize")
                                    || path.is_ident("u8")
                                    || path.is_ident("u16")
                                    || path.is_ident("u32")
                                    || path.is_ident("u64")
                                    || path.is_ident("u128")
                                    || path.is_ident("isize")
                                    || path.is_ident("i8")
                                    || path.is_ident("i16")
                                    || path.is_ident("i32")
                                    || path.is_ident("i64")
                                    || path.is_ident("i128")
                                    || path.is_ident("bool")
                                    || path.is_ident("char")
                        ) {
                            const_generic_indices.push(i);
                        }
                    }
                }
                if const_generic_indices.is_empty() {
                    continue;
                }
                let mut rng = rand::thread_rng();
                let selected_index = const_generic_indices.choose(&mut rng).unwrap();
                let mut new_generics = item_struct.generics.clone();
                if let GenericParam::Const(const_param) =
                    new_generics.params.iter_mut().nth(*selected_index).unwrap()
                {
                    const_param.ty = Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(Ident::new("usize", Span::call_site())),
                    });
                }
                item_struct.generics = new_generics;
                let mut visitor = StructInstantiationVisitor {
                    struct_ident: item_struct.ident.clone(),
                    const_index: *selected_index,
                };
                visitor.visit_file_mut(file);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters that are not of type integer, bool, or char. It randomly selects one such const generic parameter and changes its type to usize. It then updates all instantiations of the struct to use a usize constant for the modified parameter. This transformation tests the compiler's handling of const generic type changes and ensures that the code remains valid under stricter const generic constraints."
    }
}

struct StructInstantiationVisitor {
    struct_ident: Ident,
    const_index: usize,
}

impl VisitMut for StructInstantiationVisitor {
    fn visit_expr_path_mut(&mut self, node: &mut ExprPath) {
        if node.path.is_ident(&self.struct_ident) {
            if let Some(last_segment) = node.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    let mut new_args = args.clone();
                    if let Some(GenericArgument::Const(_)) = new_args.args.get(self.const_index) {
                        new_args.args[self.const_index] =
                            GenericArgument::Const(Expr::Lit(ExprLit {
                                attrs: Vec::new(),
                                lit: syn::Lit::Int(syn::LitInt::new("1", Span::call_site())),
                            }));
                        args.args = new_args.args;
                    }
                }
            }
        }
    }
}