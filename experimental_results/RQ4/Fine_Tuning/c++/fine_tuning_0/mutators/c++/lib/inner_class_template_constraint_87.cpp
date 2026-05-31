//source file
#include "../include/Inner_Class_Template_Constraint_87.h"

// ========================================================================================================
#define MUT87_OUTPUT 1

void MutatorFrontendAction_87::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto decls = CL->decls();
      bool has_inner_class_template = false;
      bool has_inner_class_template_with_constraint = false;
      for (auto decl : decls) {
        if (auto inner_class = dyn_cast<CXXRecordDecl>(decl)) {
          if (inner_class->isTemplated()) {
            has_inner_class_template = true;
            auto template_decl = inner_class->getDescribedTemplate();
            if (template_decl->getTemplateParameters()->getRequiresClause()) {
              has_inner_class_template_with_constraint = true;
              auto constraint = template_decl->getTemplateParameters()
                                    ->getRequiresClause();
              auto constraint_str = stringutils::rangetoStr(
                  *(Result.SourceManager), constraint->getSourceRange());
              llvm::outs() << constraint_str << '\n';
              constraint_str = "!(" + constraint_str + ")";
              llvm::outs() << constraint_str << '\n';
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(constraint->getSourceRange()),
                  constraint_str);
            }
          }
        }
      }
      if (!has_inner_class_template) {
        llvm::outs() << "No inner class template found\n";
        return;
      }
      if (!has_inner_class_template_with_constraint) {
        llvm::outs() << "No inner class template with constraint found\n";
        return;
      }
      auto class_name = CL->getNameAsString();
      auto outer_template = "template<typename U> struct Outer : Inner<U> {};";
      Rewrite.ReplaceText(CL->getEndLoc(), 0, "/*mut87*/" + std::string(outer_template));
    }
}
  
void MutatorFrontendAction_87::MutatorASTConsumer_87::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}