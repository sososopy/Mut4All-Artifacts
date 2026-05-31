//source file
#include "../include/Replace_Lambda_Trailing_Return_With_Decltype_79.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLocalVarDeclOrParm() || DL->isStaticLocal())
        return;
      if (DL->getType().getTypePtr()->isFunctionPointerType() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content;
      if (content.find('=') == string::npos)
        return;
      auto lhs = content.substr(0, content.find('='));
      auto rhs = content.substr(content.find('=') + 1);
      llvm::outs() << lhs << " " << rhs;
      if (rhs.find('[') == string::npos || rhs.find(']') == string::npos)
        return;
      if (rhs.find('(') == string::npos || rhs.find(')') == string::npos)
        return;
      if (rhs.find("->") == string::npos)
        return;
      auto param = rhs.substr(rhs.find('['), rhs.find(']') - rhs.find('[') + 1);
      auto args = rhs.substr(rhs.find('('), rhs.find(')') - rhs.find('(') + 1);
      auto trailing =
          rhs.substr(rhs.find("->") + 2, rhs.find('{') - rhs.find("->") - 2);
      llvm::outs() << param << " " << args << " " << trailing;
      if (trailing.find("decltype") == string::npos)
        return;
      auto newtrailing = "decltype(auto)";
      auto newrhs = param + args + "->" + newtrailing + rhs.substr(rhs.find('{'));
      auto newlhs = DL->getType().getAsString();
      auto newcontent = newlhs + " " + lhs + "=" + newrhs;
      llvm::outs() << newcontent;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), newcontent);
    }
}
  
void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}