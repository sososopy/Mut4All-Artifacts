use syn::{parse_quote, visit_mut::VisitMut, ItemStruct, Type, TypePath, Path, ItemFn, FnArg, PatType, File};

struct Replace_Const_Generic_Parameters_With_Non_Generic_Types_;

impl VisitMut for Replace_Const_Generic_Parameters_With_Non_Generic_Types_ {
    fn visit_item_struct_mut(&mut self, i: &mut ItemStruct) {
        for field in &mut i.fields {
            if let Type::Path(TypePath { path, .. }) = &mut field.ty {
                if let Path { segments, .. } = path {
                    if segments.len() == 1 && segments[0].ident == "Vec" {
                        *field.ty = parse_quote! { String };
                    }
                }
            }
        }
    }

    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        for arg in &mut i.sig.inputs {
            if let FnArg::Typed(PatType { ty, .. }) = arg {
                if let Type::Path(TypePath { path, .. }) = ty.as_ref() {
                    if let Path { segments, .. } = path {
                        if segments.len() == 1 && segments[0].ident == "Vec" {
                            *ty = parse_quote! { String };
                        }
                    }
                }
            }
        }
    }
}

impl Replace_Const_Generic_Parameters_With_Non_Generic_Types_ {
    fn mutate(&mut self, file_ast: &mut File) {
        visit_mut::visit_file_mut(self, file_ast);
    }
}