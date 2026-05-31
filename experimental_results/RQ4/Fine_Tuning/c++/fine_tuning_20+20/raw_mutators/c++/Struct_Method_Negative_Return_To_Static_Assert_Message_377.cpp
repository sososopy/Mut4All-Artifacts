//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Method_Negative_Return_To_Static_Assert_Message_377
 */ 
class MutatorFrontendAction_377 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(377)

private:
    class MutatorASTConsumer_377 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_377(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::CXXRecordDecl *> cur_structs;
    };
};

//source file
#include "../include/struct_method_negative_return_to_static_assert_message_377.h"

// ========================================================================================================
#define MUT377_OUTPUT 1

void MutatorFrontendAction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isStruct())
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_structs.push_back(DL);
  } else if (auto *SL = Result.Nodes.getNodeAs<clang::StaticAssertDecl>(
                 "StaticAsserts")) {
    if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   SL->getLocation()))
      return;
    auto msg = SL->getMessage();
    if (!msg)
      return;
    auto msgtype = msg->getType();
    if (!msgtype->isStructureOrClassType())
      return;
    auto msgstr = msgtype.getAsString();
    llvm::outs() << "msgstr: " << msgstr << '\n';
    for (auto DL : cur_structs) {
      if (DL->getNameAsString() == msgstr) {
        auto methods = DL->methods();
        for (auto method : methods) {
          if (method->isConstexpr()) {
            auto rettype = method->getReturnType();
            if (rettype->isIntegralOrEnumerationType()) {
              auto body = method->getBody();
              if (!body)
                return;
              auto bodystr = stringutils::rangetoStr(
                  *(Result.SourceManager), body->getSourceRange());
              if (bodystr.find("return -") != string::npos) {
                llvm::outs() << "bodystr: " << bodystr << '\n';
                bodystr = "/*mut377*/{return 0;}";
                Rewrite.ReplaceText(
                    CharSourceRange::getTokenRange(body->getSourceRange()),
                    bodystr);
              }
            }
          }
        }
      }
    }
  }
}
  
void MutatorFrontendAction_377::MutatorASTConsumer_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
  auto static_assert_matcher = staticAssertDecl().bind("StaticAsserts");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(static_assert_matcher, &callback);
  matchFinder.matchAST(Context);
}