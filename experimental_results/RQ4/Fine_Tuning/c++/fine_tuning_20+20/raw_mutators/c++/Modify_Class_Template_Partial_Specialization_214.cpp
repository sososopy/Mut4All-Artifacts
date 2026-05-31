//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Partial_Specialization_214
 */ 
class MutatorFrontendAction_214 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(214)

private:
    class MutatorASTConsumer_214 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_214(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> typename_list;
    };
};

//source file
#include "../include/Modify_Class_Template_Partial_Specialization_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecialization")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      typename_list.push_back(content);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (typename_list.empty())
        return;
      size_t index = getrandom::getRandomIndex(typename_list.size() - 1);
      auto ins = typename_list[index] + "\n";
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut214*/" + ins);
    }
}
  
void MutatorFrontendAction_214::MutatorASTConsumer_214::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplatePartialSpecializationDecl().bind(
        "PartialSpecialization");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}