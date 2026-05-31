//source file
#include "../include/defaulted_assignment_operator_with_explicit_object_parameter_in_derived_struct_351.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStruct() == false || DL->isCompleteDefinition() == false)
        return;
      bool is_derived = false;
      for (auto base : base_structs) {
        if (DL->isDerivedFrom(base)) {
          is_derived = true;
          break;
        }
      }
      if (is_derived == false)
        return;
      llvm::outs() << DL->getNameAsString() << '\n';
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          llvm::outs() << "using\n";
        }
      }
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isCopyAssignmentOperator()) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 method->getSourceRange());
          llvm::outs() << content << '\n';
          auto pos = content.find('(');
          if (pos != string::npos) {
            content.insert(pos + 1, "Mut351_base &");
            content += "=default;";
            content = "/*mut351*/" + content;
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(method->getSourceRange()),
                content);
          }
        }
      }
    } else if (auto *BL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
      if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BL->getLocation()))
        return;
      if (BL->isStruct() == false || BL->isCompleteDefinition() == false)
        return;
      base_structs.push_back(BL);
    }
}

void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}