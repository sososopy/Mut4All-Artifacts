//source file
#include "../include/Modify_Member_Function_With_Explicit_Object_Parameter_351.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto bases = DL->bases();
      if (bases.begin() == bases.end())
        return;
      auto base = bases.begin();
      auto base_type = base->getType();
      if (base_type->isDependentType())
        return;
      auto base_name = base_type.getAsString();
      llvm::outs() << base_name << '\n';
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      for (auto decl : decls) {
        if (decl->getKind() == Decl::Kind::CXXMethod) {
          auto method = dyn_cast<CXXMethodDecl>(decl);
          if (method->isImplicit())
            continue;
          if (method->isStatic())
            continue;
          if (method->isCopyAssignmentOperator()) {
            auto method_name = stringutils::rangetoStr(
                *(Result.SourceManager), method->getSourceRange());
            llvm::outs() << method_name << '\n';
            auto new_method_name = method_name;
            new_method_name.insert(new_method_name.find('(') + 1,
                                   "this " + base_name + " &");
            llvm::outs() << new_method_name << '\n';
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                    method->getSourceRange()),
                                new_method_name);
          }
        }
      }
    }
  }
  
void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}