//source file
#include "../include/Modify_Friend_Function_Default_Argument_In_Template_219.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isTemplated() == false)
        return;
      if (DL->isLambda())
        return;
      recordDecl = DL;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Friends")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getFriendObjectKind() == clang::Decl::FriendObjectKind::FOK_None)
        return;
      if (FD->isInIdentifierNamespace(Decl::IDNS_LocalExtern))
        return;
      if (FD->isInIdentifierNamespace(Decl::IDNS_OrdinaryFriend))
        return;
      auto params = FD->parameters();
      for (auto param : params) {
        if (param->getType()->isRecordType()) {
          if (param->getType()->getAsCXXRecordDecl() == recordDecl) {
            auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
            if (content.find('=') != string::npos)
              return;
            content.insert(content.rfind(')'), "=0");
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(FD->getSourceRange()), content);
          }
        }
      }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("Call")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CE->getSourceRange());
      if (content.find('=') != string::npos)
        return;
      if (content.find(',') != string::npos) {
        content.erase(content.find(','), 2);
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(CE->getSourceRange()), content);
    }
  }

void MutatorFrontendAction_219::MutatorASTConsumer_219::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto friend_matcher = functionDecl().bind("Friends");
    auto call_matcher = callExpr().bind("Call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(friend_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}