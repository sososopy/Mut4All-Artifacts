//source file
#include "../include/Insert_Forward_Declaration_And_Definition_Of_Struct_In_Function_358.h"

// ========================================================================================================
#define MUT358_OUTPUT 1

void MutatorFrontendAction_358::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      if (body->children().begin() == body->children().end())
        return;
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      llvm::outs() << body_str << '\n';
      auto id = getrandom::getRandomIndex(100);
      auto struct_def = "struct Mut_" + std::to_string(id) + ";\n";
      struct_def += "struct Mut_" + std::to_string(id) + "{int x;};\n";
      struct_def = "/*mut358*/" + struct_def;
      body_str.insert(1, struct_def);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), body_str);
    }
}
  
void MutatorFrontendAction_358::MutatorASTConsumer_358::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}