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

pub struct Modify_PhantomData_Usage_6;

impl Mutator for Modify_PhantomData_Usage_6 {
    fn name(&self) -> &str {
        "Modify_PhantomData_Usage_6"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TypeCollector {
            types: Vec<String>,
        }

        impl<'ast> Visit<'ast> for TypeCollector {
            fn visit_type_path(&mut self, node: &'ast TypePath) {
                let type_str = node.to_token_stream().to_string();
                self.types.push(type_str);
                syn::visit::visit_type_path(self, node);
            }
        }

        let mut collector = TypeCollector { types: Vec::new() };
        collector.visit_file(file);

        let mut rng = thread_rng();
        let selected_type = collector.types.choose(&mut rng).cloned();

        if let Some(selected_type) = selected_type {
            for item in &mut file.items {
                if let Item::Struct(item_struct) = item {
                    for field in &mut item_struct.fields {
                        if let Type::Path(type_path) = &mut field.ty {
                            if type_path.path.segments.last().unwrap().ident == "PhantomData" {
                                if let PathArguments::AngleBracketed(angle_bracketed) =
                                    &mut type_path.path.segments.last_mut().unwrap().arguments
                                {
                                    if let Some(GenericArgument::Type(Type::Path(type_path))) =
                                        angle_bracketed.args.first_mut()
                                    {
                                        *type_path = parse_quote!(#selected_type);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs using PhantomData and replaces the type parameter with a randomly selected existing type from the seed program, aiming to stress the compiler's type resolution and associated item handling."
    }
}