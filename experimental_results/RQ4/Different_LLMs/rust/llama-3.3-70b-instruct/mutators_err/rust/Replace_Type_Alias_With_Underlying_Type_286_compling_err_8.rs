use syn::{File, Item, Type, TypePath, Path, FnArg, ImplItem};
use std::collections::HashMap;

struct Replace_Type_Alias_With_Underlying_Type_286;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Type_Alias_With_Underlying_Type_286 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Underlying_Type_286"
    }

    fn mutate(&self, file: &mut File) {
        let mut type_aliases = HashMap::new();

        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::Path(TypePath {
                    qself: None,
                    path,
                    ..
                }) = *item_type.ty
                {
                    if let Some(segment) = path.segments.last() {
                        type_aliases.insert(segment.ident.to_string(), item_type.ty.clone());
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path,
                            ..
                        }) = *pat_type.ty
                        {
                            if let Some(segment) = path.segments.last() {
                                if let Some(underlying_type) = type_aliases.get(&segment.ident.to_string()) {
                                    *pat_type.ty = (*underlying_type).clone();
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for arg in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path,
                                    ..
                                }) = *pat_type.ty
                                {
                                    if let Some(segment) = path.segments.last() {
                                        if let Some(underlying_type) = type_aliases.get(&segment.ident.to_string()) {
                                            *pat_type.ty = (*underlying_type).clone();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Type::Path(TypePath {
                        qself: None,
                        path,
                        ..
                    }) = field.ty
                    {
                        if let Some(segment) = path.segments.last() {
                            if let Some(underlying_type) = type_aliases.get(&segment.ident.to_string()) {
                                field.ty = (*underlying_type).clone();
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with their underlying types. It aims to test the compiler's handling of type aliases and their underlying types, potentially revealing issues with the type system, const evaluation, and other related features."
    }
}