//source file
#include "../include/Delete_Member_And_Friend_Functions_And_Ctor_262.h"

// ========================================================================================================
#define MUT262_OUTPUT 1

void MutatorFrontendAction_262::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (CL->isCompleteDefinition() == false)
      return;

    cur_classes.push_back(CL);
  } else if (auto *IL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Incomplete")) {
    if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   IL->getLocation()))
      return;
    if ((!IL->isStruct() && !IL->isClass()) || IL->isLambda())
      return;
    if (IL->isCompleteDefinition() == true)
      return;
    cur_classes.push_back(IL);
  } else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                 "MemberFunctions")) {
    if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MD->getLocation()))
      return;
    if (MD->isLambdaStaticInvoker())
      return;
    cur_methods.push_back(MD);
  } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>(
                 "Constructors")) {
    if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
      return;
    cur_ctors.push_back(CD);
  } else if (auto *FD =
                 Result.Nodes.getNodeAs<clang::FriendDecl>("FriendDecls")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    cur_friends.push_back(FD);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DoDelete")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (DL->isCompleteDefinition() == false)
      return;

    if (cur_classes.empty())
      return;
    auto target = cur_classes[getrandom::getRandomIndex(cur_classes.size() - 1)];
    if (!target->isCompleteDefinition()) {
      llvm::outs() << "/*mut262*/\n";
      return;
    }
    auto methods = target->methods();
    size_t method_num = 0;
    for (auto method : methods) {
      if (method->isLambdaStaticInvoker())
        continue;
      method_num++;
    }
    if (method_num == 0) {
      llvm::outs() << "/*mut262*/\n";
      return;
    }

    if (method_num != 0) {
      auto target_method =
          cur_methods[getrandom::getRandomIndex(cur_methods.size() - 1)];
      auto method_content = stringutils::rangetoStr(
          *(Result.SourceManager), target_method->getSourceRange());
      if (target_method->isDeleted())
        return;
      if (target_method->isLambdaStaticInvoker())
        return;
      llvm::outs() << "/*mut262*/\n";
      if (target_method->isDefaulted()) {
        method_content += "=delete;";
      } else {
        method_content += " = delete;";
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target_method->getSourceRange()),
                          method_content);
    }

    auto ctors = target->ctors();
    size_t ctor_num = 0;
    for (auto ctor : ctors) {
      ctor_num++;
    }
    if (ctor_num == 0) {
      Rewrite.ReplaceText(target->getEndLoc(), 0,
                          "/*mut262*/\npublic: " + target->getNameAsString() +
                              "() = delete;\n");
    } else {
      auto target_ctor =
          cur_ctors[getrandom::getRandomIndex(cur_ctors.size() - 1)];
      auto ctor_content = stringutils::rangetoStr(
          *(Result.SourceManager), target_ctor->getSourceRange());
      if (target_ctor->isDeleted())
        return;
      if (target_ctor->isDefaulted()) {
        ctor_content += "=delete;";
      } else {
        ctor_content += " = delete;";
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target_ctor->getSourceRange()),
                          ctor_content);
    }

    auto friends = target->friends();
    size_t friend_num = 0;
    for (auto friend_ : friends) {
      friend_num++;
    }
    if (friend_num == 0) {
      Rewrite.ReplaceText(target->getEndLoc(), 0,
                          "/*mut262*/\nfriend void mut262()=delete;\n");
    } else {
      auto target_friend =
          cur_friends[getrandom::getRandomIndex(cur_friends.size() - 1)];
      auto friend_content = stringutils::rangetoStr(
          *(Result.SourceManager), target_friend->getSourceRange());
      friend_content += "=delete;";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target_friend->getSourceRange()),
                          friend_content);
    }
  }
}

void MutatorFrontendAction_262::MutatorASTConsumer_262::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
  auto incomplete_matcher = cxxRecordDecl().bind("Incomplete");
  auto member_matcher = cxxMethodDecl().bind("MemberFunctions");
  auto ctor_matcher = cxxConstructorDecl().bind("Constructors");
  auto friend_matcher = friendDecl().bind("FriendDecls");
  auto delete_matcher = cxxRecordDecl().bind("DoDelete");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.addMatcher(incomplete_matcher, &callback);
  matchFinder.addMatcher(member_matcher, &callback);
  matchFinder.addMatcher(ctor_matcher, &callback);
  matchFinder.addMatcher(friend_matcher, &callback);
  matchFinder.addMatcher(delete_matcher, &callback);
  matchFinder.matchAST(Context);
}