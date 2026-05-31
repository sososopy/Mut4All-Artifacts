//source file
#include "../include/Target_Incomplete_Struct_Member_Access_373.h"

// ========================================================================================================
#define MUT373_OUTPUT 1

void MutatorFrontendAction_373::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto params = FD->parameters();
      for (auto param : params) {
        if (param->getType()->isPointerType()) {
          auto pointeeType =
              param->getType()->getPointeeType().getTypePtrOrNull();
          if (pointeeType && pointeeType->isStructureType()) {
            auto structDecl = pointeeType->getAsRecordDecl();
            if (structDecl && !structDecl->isCompleteDefinition()) {
              auto paramName = param->getNameAsString();
              if (paramName == "")
                continue;
              auto body = FD->getBody();
              auto memberAccessMatcher =
                  memberExpr(hasObjectExpression(ignoringImpCasts(
                                 declRefExpr(to(varDecl(hasName(paramName)))))))
                      .bind("memberAccess");
              auto matches = match(memberAccessMatcher, *body,
                                   *Result.Context);
              for (auto match : matches) {
                if (auto memberAccess =
                        match.getNodeAs<MemberExpr>("memberAccess")) {
                  auto memberName = memberAccess->getMemberNameInfo().getAsString();
                  llvm::outs() << "Found member access: " << memberName << "\n";
                  auto newMemberName = "nonExistentMember";
                  Rewrite.ReplaceText(memberAccess->getMemberNameInfo().getSourceRange(), newMemberName);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_373::MutatorASTConsumer_373::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}