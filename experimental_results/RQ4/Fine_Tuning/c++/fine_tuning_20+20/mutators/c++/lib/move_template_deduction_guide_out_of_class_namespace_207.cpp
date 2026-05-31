//source file
#include "../include/move_template_deduction_guide_out_of_class_namespace_207.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_207::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      llvm::outs() << "template found\n";
      cur_templates.push_back(TP);
    } else if (auto *DG = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "DeductionGuides")) {
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DG->getSourceRange());
      llvm::outs() << "content:\n" << content << '\n';
      bool found = false;
      for (size_t i = 0; i < cur_templates.size(); ++i) {
        if (DG->getNameAsString() == cur_templates[i]->getNameAsString()) {
          found = true;
          break;
        }
      }
      if (found) {
        Rewrite.ReplaceText(DG->getSourceRange(), "");
        Rewrite.ReplaceText(DG->getEndLoc(), 0, "/*mut207*/" + content + "\n");
      }
    }
}

void MutatorFrontendAction_207::MutatorASTConsumer_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = classTemplateDecl().bind("Templates");
    auto deduction_guide_matcher = classTemplateSpecializationDecl().bind("DeductionGuides");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(deduction_guide_matcher, &callback);
    matchFinder.matchAST(Context);
}