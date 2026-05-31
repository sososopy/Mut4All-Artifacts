//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Initializer_List_Begin_422
 */ 
class MutatorFrontendAction_422 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(422)

private:
    class MutatorASTConsumer_422 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_422(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Modify_Initializer_List_Begin_422.h"

// ========================================================================================================
#define MUT422_OUTPUT 1

void MutatorFrontendAction_422::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("InitListBegin")) {
      if (!MT || !Result.SourceManager->isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      if (!MT->getMethodDecl()->getNameAsString().compare("begin") == 0)
        return;
      if (!MT->getRecordDecl()->getNameAsString().compare("initializer_list") ==
          0)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      auto element_type =
          MT->getRecordDecl()
              ->getDescribedClassTemplate()
              ->getTemplateParameters()
              ->getParam(0)
              ->getNameAsString();
      auto il = "std::initializer_list<" + element_type + ">{0,1,2,3,4}";
      auto mutated_content = il + ".begin()";
      llvm::outs() << content << "->" << mutated_content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          mutated_content);
    }
}
  
void MutatorFrontendAction_422::MutatorASTConsumer_422::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr(
                       hasObjectExpression(hasType(cxxRecordDecl(hasName(
                           "std::initializer_list"))))).bind("InitListBegin");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}