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

pub struct Change_Const_Generic_To_Fn_Ptr_385;

impl Mutator for Change_Const_Generic_To_Fn_Ptr_385 {
    fn name(&self) -> &str {
        "Change_Const_Generic_To_Fn_Ptr_385"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found_function = None;
        let mut visitor = FunctionFinder {
            functions: Vec::new(),
        };
        visitor.visit_file(file);
        for func in visitor.functions {
            if let Some((_, sig)) = Some(func) {
                let inputs = &sig.inputs;
                let output = &sig.output;
                let return_type = match output {
                    ReturnType::Default => None,
                    ReturnType::Type(_, ty) => Some(ty),
                };
                found_function = Some((inputs.clone(), return_type.clone()));
                break;
            }
        }
        let (dummy_inputs, dummy_output) = if let Some((inputs, output)) = found_function {
            (inputs, output)
        } else {
            (Punctuated::new(), None)
        };
        let mut applied = false;
        for item in &mut file.items {
            if applied {
                break;
            }
            match item {
                Item::Struct(item_struct) => {
                    if let Some(pos) = find_const_generic(&mut item_struct.generics.params) {
                        if let Some(new_type) = construct_fn_ptr_type(&dummy_inputs, dummy_output.as_ref()) {
                            if let GenericParam::Const(const_param) = &mut item_struct.generics.params[pos] {
                                const_param.ty = new_type;
                                applied = true;
                            }
                        }
                    }
                }
                Item::Enum(item_enum) => {
                    if let Some(pos) = find_const_generic(&mut item_enum.generics.params) {
                        if let Some(new_type) = construct_fn_ptr_type(&dummy_inputs, dummy_output.as_ref()) {
                            if let GenericParam::Const(const_param) = &mut item_enum.generics.params[pos] {
                                const_param.ty = new_type;
                                applied = true;
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    if let Some(pos) = find_const_generic(&mut item_fn.sig.generics.params) {
                        if let Some(new_type) = construct_fn_ptr_type(&dummy_inputs, dummy_output.as_ref()) {
                            if let GenericParam::Const(const_param) = &mut item_fn.sig.generics.params[pos] {
                                const_param.ty = new_type;
                                applied = true;
                            }
                        }
                    }
                }
                _ => {}
            }
        }
        if !applied && dummy_inputs.is_empty() {
            let dummy_fn: ItemFn = parse_quote! {
                fn dummy() -> () {}
            };
            file.items.push(Item::Fn(dummy_fn));
            for item in &mut file.items {
                if applied {
                    break;
                }
                match item {
                    Item::Struct(item_struct) => {
                        if let Some(pos) = find_const_generic(&mut item_struct.generics.params) {
                            if let GenericParam::Const(const_param) = &mut item_struct.generics.params[pos] {
                                const_param.ty = parse_quote!(fn() -> ());
                                applied = true;
                            }
                        }
                    }
                    Item::Enum(item_enum) => {
                        if let Some(pos) = find_const_generic(&mut item_enum.generics.params) {
                            if let GenericParam::Const(const_param) = &mut item_enum.generics.params[pos] {
                                const_param.ty = parse_quote!(fn() -> ());
                                applied = true;
                            }
                        }
                    }
                    Item::Fn(item_fn) => {
                        if let Some(pos) = find_const_generic(&mut item_fn.sig.generics.params) {
                            if let GenericParam::Const(const_param) = &mut item_fn.sig.generics.params[pos] {
                                const_param.ty = parse_quote!(fn() -> ());
                                applied = true;
                            }
                        }
                    }
                    _ => {}
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes a const generic parameter type from a supported type (e.g., usize, i32, bool, char) to a function pointer type. It first scans the seed program for an existing function to use as the function pointer signature. If found, it constructs a corresponding fn pointer type and replaces the const generic type in the first eligible struct, enum, or function definition. If no suitable function exists, it introduces a trivial dummy function and uses its signature. This transformation aims to trigger internal compiler errors related to unsupported const generic types and constant evaluation, exploiting the generic_const_exprs feature."
    }
}

fn find_const_generic(params: &mut Punctuated<GenericParam, Comma>) -> Option<usize> {
    for (i, param) in params.iter_mut().enumerate() {
        if let GenericParam::Const(const_param) = param {
            if let Type::Path(type_path) = &const_param.ty {
                if let Some(segment) = type_path.path.segments.last() {
                    let ident_str = segment.ident.to_string();
                    if ident_str == "usize" || ident_str == "i32" || ident_str == "bool" || ident_str == "char" {
                        return Some(i);
                    }
                }
            }
        }
    }
    None
}

fn construct_fn_ptr_type(inputs: &Punctuated<FnArg, Comma>, output: Option<&Box<Type>>) -> Option<Type> {
    let mut fn_inputs = Punctuated::<Type, Comma>::new();
    for input in inputs {
        match input {
            FnArg::Typed(PatType { ty, .. }) => {
                fn_inputs.push(*ty.clone());
            }
            FnArg::Receiver(_) => {
                return None;
            }
        }
    }
    let fn_output = match output {
        Some(ty) => *ty.clone(),
        None => parse_quote!(()),
    };
    Some(parse_quote!(fn(#fn_inputs) -> #fn_output))
}

struct FunctionFinder {
    functions: Vec<(Option<Ident>, Signature)>,
}

impl Visit<'_> for FunctionFinder {
    fn visit_item_fn(&mut self, node: &ItemFn) {
        self.functions.push((Some(node.sig.ident.clone()), node.sig.clone()));
        syn::visit::visit_item_fn(self, node);
    }
    fn visit_impl_item_fn(&mut self, node: &ImplItemFn) {
        self.functions.push((Some(node.sig.ident.clone()), node.sig.clone()));
        syn::visit::visit_impl_item_fn(self, node);
    }
}