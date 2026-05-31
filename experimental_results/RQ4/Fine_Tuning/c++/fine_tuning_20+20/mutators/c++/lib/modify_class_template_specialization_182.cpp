//source file
#include "../include/modify_class_template_specialization_182.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      llvm::outs() << "class template\n";
      cur_templates.push_back(CL);
    } else if (auto *SP = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
                   "Specializations")) {
      if (!SP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SP->getLocation()))
        return;
      llvm::outs() << "class template specialization\n";
      auto specs =
          stringutils::rangetoStr(*(Result.SourceManager), SP->getSourceRange());
      llvm::outs() << specs << '\n';
      for (std::size_t i = 0; i < cur_templates.size(); ++i) {
        auto tps = cur_templates[i]->getTemplateParameters();
        if (tps->size() == 1) {
          auto tp = tps->getParam(0);
          llvm::outs() << tp->getNameAsString() << '\n';
          if (tp->getNameAsString() == specs) {
            llvm::outs() << "match\n";
            if (tp->isTemplateParameter()) {
              llvm::outs() << "template parameter\n";
              auto ttp = dyn_cast<TemplateTypeParmDecl>(tp);
              if (ttp->isParameterPack() == false) {
                llvm::outs() << "not parameter pack\n";
                string ins = ", class U";
                string replace = "T, U";
                Rewrite.ReplaceText(SP->getBeginLoc(), 0, ins);
                Rewrite.ReplaceText(SP->getLocation(), replace);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto specialization_matcher =
        classTemplateSpecializationDecl().bind("Specializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}