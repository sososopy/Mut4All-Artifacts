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

pub struct Modify_Const_Generic_To_Unsupported_Type_400;

impl Mutator for Modify_Const_Generic_To_Unsupported_Type_400 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_To_Unsupported_Type_400"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generics = false;
                for param in &item_struct.generics.params {
                    if matches!(param, GenericParam::Const(_)) {
                        has_const_generics = true;
                        break;
                    }
                }
                if !has_const_generics {
                    continue;
                }
                let mut const_generic_idents = Vec::new();
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_generic_idents.push(const_param.ident.clone());
                    }
                }
                let mut rng = thread_rng();
                const_generic_idents.shuffle(&mut rng);
                if let Some(target_ident) = const_generic_idents.first() {
                    let mut visitor = ConstGenericReplacer {
                        target_ident,
                        replacement: parse_quote! { [[usize; 3]; 3] },
                    };
                    visitor.visit_item_struct_mut(item_struct);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters and replaces one of these parameters with a multi-dimensional array type, which is currently unsupported as a const generic type in stable Rust. By introducing an unsupported const generic type, the mutator aims to provoke compilation errors related to const generic constraints, testing the compiler's handling of type validation and error reporting in the context of const generics."
    }
}

struct ConstGenericReplacer<'a> {
    target_ident: &'a Ident,
    replacement: Type,
}

impl<'a> VisitMut for ConstGenericReplacer<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            if type_path.qself.is_none() && type_path.path.segments.len() == 1 {
                let segment = &type_path.path.segments[0];
                if segment.ident == *self.target_ident {
                    *node = self.replacement.clone();
                    return;
                }
            }
        }
        visit_mut::visit_type_mut(self, node);
    }
}