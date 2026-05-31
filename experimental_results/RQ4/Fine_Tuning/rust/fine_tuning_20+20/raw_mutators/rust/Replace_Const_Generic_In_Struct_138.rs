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

pub struct Replace_Const_Generic_In_Struct_138;

impl Mutator for Replace_Const_Generic_In_Struct_138 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_In_Struct_138"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_params = Vec::new();
                for (index, param) in item_struct.generics.params.iter().enumerate() {
                    if let GenericParam::Const(_) = param {
                        const_params.push(index);
                    }
                }
                if const_params.is_empty() {
                    continue;
                }
                let target_index = const_params[0];
                let new_ident = Ident::new("U", Span::call_site());
                item_struct.generics.params[target_index] =
                    GenericParam::Type(TypeParam::from(new_ident.clone()));
                let target_const_ident = if let GenericParam::Const(const_param) =
                    &item_struct.generics.params[target_index]
                {
                    const_param.ident.clone()
                } else {
                    continue;
                };
                struct StructVisitor {
                    target_const_ident: Ident,
                    new_ident: Ident,
                }
                impl VisitMut for StructVisitor {
                    fn visit_type_mut(&mut self, ty: &mut Type) {
                        if let Type::Path(type_path) = ty {
                            if type_path.qself.is_none()
                                && type_path.path.segments.len() == 1
                                && type_path.path.segments[0].ident == self.target_const_ident
                            {
                                *ty = Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(self.new_ident.clone()),
                                });
                            }
                        }
                        syn::visit_mut::visit_type_mut(self, ty);
                    }
                }
                let mut visitor = StructVisitor {
                    target_const_ident,
                    new_ident,
                };
                visitor.visit_item_struct_mut(item_struct);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generics. It replaces one const parameter with a generic type parameter and updates all usages of the const parameter within the struct's fields and trait implementations to use the new generic type. This transformation stresses the compiler's handling of generic parameter substitution and type resolution, particularly in contexts where const generics are expected, potentially revealing weaknesses in the compiler's ability to adapt to changes in generic parameter kinds."
    }
}