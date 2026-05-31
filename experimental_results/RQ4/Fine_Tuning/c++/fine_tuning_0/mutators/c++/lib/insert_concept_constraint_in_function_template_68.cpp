//source file
#include "../include/Insert_Concept_Constraint_In_Function_Template_68.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->isThisDeclarationADefinition() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FT->getSourceRange());
      auto tpl = FT->getTemplateParameters();
      if (tpl->size() == 0)
        return;
      auto tp = tpl->getParam(0);
      auto tp_name = tp->getName().str();
      llvm::outs() << tp_name << '\n';
      if (tp_name == "")
        return;
      content.insert(content.find(tp_name) + tp_name.size(), " /*mut68*/");
      content.insert(content.find(tp_name) + tp_name.size() + 10,
                     " requires ConceptName");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}