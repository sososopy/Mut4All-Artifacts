//source file
#include "../include/Add_Concept_Requirements_To_Function_109.h"

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation() || FD->isTemplated())
        return;
      if (FD->isOverloadedOperator())
        return;
      if (!FD->doesThisDeclarationHaveABody())
        return;
      if (FD->isMain())
        return;
      if (FD->getNameAsString() == "")
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isVoidType())
        return;
      auto params = FD->parameters();
      if (params.empty())
        return;
      auto param1 = params[0];
      auto param1_type = param1->getType();
      if (param1_type->isUndeducedType())
        return;
      if (!param1_type->isBuiltinType())
        return;
      auto param2 = params[1];
      auto param2_type = param2->getType();
      if (param2_type->isUndeducedType())
        return;
      if (!param2_type->isBuiltinType())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FD->getSourceRange());
      string ins = "requires requires(" + param1_type.getAsString() + " " +
                   param1->getNameAsString() + ", " +
                   param2_type.getAsString() + " " + param2->getNameAsString() +
                   "){ { " + param1->getNameAsString() + " + " +
                   param2->getNameAsString() + " } -> std::convertible_to<" +
                   FD->getReturnType().getAsString() + ">; } ";
      content.insert(content.find('{'), ins);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}