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

pub struct Modify_Struct_With_Generic_Const_Parameters_97;

impl Mutator for Modify_Struct_With_Generic_Const_Parameters_97 {
    fn name(&self) -> &str {
        "Modify_Struct_With_Generic_Const_Parameters_97"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generic_with_lifetime = false;
                let mut const_generic_index = 0;
                let mut lifetime_index = 0;
                for (index, param) in item_struct.generics.params.iter().enumerate() {
                    match param {
                        GenericParam::Lifetime(_) => lifetime_index = index,
                        GenericParam::Const(_) => {
                            if let GenericParam::Const(const_param) = param {
                                if let Type::Path(type_path) = &const_param.ty {
                                    if type_path
                                        .path
                                        .segments
                                        .iter()
                                        .any(|seg| seg.ident == "SomeType")
                                    {
                                        has_const_generic_with_lifetime = true;
                                        const_generic_index = index;
                                    }
                                }
                            }
                        }
                        _ => {}
                    }
                }
                if has_const_generic_with_lifetime {
                    let mut new_generics = item_struct.generics.clone();
                    if let Some(GenericParam::Const(const_param)) =
                        new_generics.params.get_mut(const_generic_index)
                    {
                        const_param.ty = parse_quote!(SomeType<'static>);
                    }
                    let new_struct: ItemStruct = ItemStruct {
                        attrs: item_struct.attrs.clone(),
                        vis: item_struct.vis.clone(),
                        struct_token: item_struct.struct_token,
                        ident: item_struct.ident.clone(),
                        generics: new_generics,
                        fields: item_struct.fields.clone(),
                        semi_token: item_struct.semi_token,
                    };
                    *item_struct = new_struct;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters that involve lifetimes. It modifies the const parameter to use a fixed `'static` lifetime, creating a mismatch with the struct's generic lifetimes. This transformation exploits known compiler weaknesses in const generics and lifetime resolution to increase the likelihood of triggering ICEs or other compile-time errors."
    }
}