//source file
#include "../include/add_static_member_with_complex_initializer_325.h"

// ========================================================================================================
#define MUT325_OUTPUT 1

void MutatorFrontendAction_325::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      cur_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile( DL->getLocation())) return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() == Decl::IdentifierNamespace::IDNS_Using) {
        }
      }
      if (DL->isCompleteDefinition() == false) return;
      for (std::size_t j = 0; j < cur_classes.size(); ++j) {
        if (DL->isDerivedFrom(cur_classes[j])) {
          auto fields = cur_classes[j]->fields();
          std::vector<FieldDecl *> public_field;
          std::vector<FieldDecl *> protected_field;
          for (auto field : fields) {
            if (field->getAccess() == AccessSpecifier::AS_protected) protected_field.push_back(field);
            else if (field->getAccess() == AccessSpecifier::AS_public) public_field.push_back(field);
          }
          if (!protected_field.empty()) {
            llvm::outs() << "/*mut325*/\npublic:\n";
            string classname = cur_classes[j]->getNameAsString();
            for (auto field : protected_field) {
              string fieldname = field->getNameAsString();
              llvm::outs() << "using " + classname + " :: " + fieldname + ";\n";
            }
          }
          if (!public_field.empty()) {
            llvm::outs() << "protected:\n";
            string classname = cur_classes[j]->getNameAsString();
            for (auto field : public_field) {
              string fieldname = field->getNameAsString();
              llvm::outs() << "using " + classname + " :: " + fieldname + ";\n";
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_325::MutatorASTConsumer_325::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}