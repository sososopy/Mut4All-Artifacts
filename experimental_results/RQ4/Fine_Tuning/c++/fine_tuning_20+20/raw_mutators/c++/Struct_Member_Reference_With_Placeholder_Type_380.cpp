//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Member_Reference_With_Placeholder_Type_380
 */ 
class MutatorFrontendAction_380 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(380)

private:
    class MutatorASTConsumer_380 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_380(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> cur_structs;
    };
};

//source file
#include "../include/struct_member_reference_with_placeholder_type_380.h"

// ========================================================================================================
#define MUT380_OUTPUT 1

void MutatorFrontendAction_380::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (ST->isStruct() == false)
        return;
      auto name = ST->getNameAsString();
      cur_structs.push_back(name);
    } else if (auto *MR = Result.Nodes.getNodeAs<clang::MemberExpr>("Members")) {
      if (!MR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MR->getBeginLoc()))
        return;
      auto base = MR->getBase();
      auto base_type = base->getType();
      auto base_type_str = base_type.getAsString();
      llvm::outs() << base_type_str << '\n';
      for (auto st : cur_structs) {
        if (base_type_str.find(st) != string::npos) {
          auto member_name = MR->getMemberNameInfo().getAsString();
          auto target = "typename ::" + member_name;
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(MR->getSourceRange()), target);
        }
      }
    }
}
  
void MutatorFrontendAction_380::MutatorASTConsumer_380::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto member_matcher = memberExpr().bind("Members");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(member_matcher, &callback);
    matchFinder.matchAST(Context);
}