//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Access_Specifier_In_Class_Declaration_313
 */ 
class MutatorFrontendAction_313 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(313)

private:
    class MutatorASTConsumer_313 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_313(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Access_Specifier_In_Class_Declaration_313.h"

// ========================================================================================================
#define MUT313_OUTPUT 1

void MutatorFrontendAction_313::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto methods = CL->methods();
      if (methods.empty())
        return;
      size_t index = getrandom::getRandomIndex(methods.size() - 1);
      auto target = methods[index];
      auto target_range = target->getSourceRange();
      auto target_code = stringutils::rangetoStr(*(Result.SourceManager),
                                                 target->getSourceRange());
      if (target_code.find("virtual") == string::npos) {
        target_code = "virtual " + target_code;
      } else {
        target_code = target_code + "/*mut313*/static";
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target_range),
                          target_code);
    }
}
  
void MutatorFrontendAction_313::MutatorASTConsumer_313::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}