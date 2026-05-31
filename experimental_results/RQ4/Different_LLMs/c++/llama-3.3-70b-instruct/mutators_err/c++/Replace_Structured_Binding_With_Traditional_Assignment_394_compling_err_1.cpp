//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Structured_Binding_With_Traditional_Assignment_394
 */ 
class MutatorFrontendAction_Replace_Structured_Binding_With_Traditional_Assignment_394 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Structured_Binding_With_Traditional_Assignment_394)

private:
    class MutatorASTConsumer_Replace_Structured_Binding_With_Traditional_Assignment_394 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Structured_Binding_With_Traditional_Assignment_394(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Structured_Binding_With_Traditional_Assignment_394.h"

// ========================================================================================================
#define MUT_REPLACE_STRUCTURED_BINDING_WITH_TRADITIONAL_ASSIGNMENT_394_OUTPUT 1

void MutatorFrontendAction_Replace_Structured_Binding_With_Traditional_Assignment_394::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRefExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = declaration;
      if (auto *PDecl = dyn_cast<clang::ParenDecl>(MT->getDecl())) {
        if (auto *InitList = dyn_cast<clang::InitListExpr>(PDecl->getInit())) {
          if (InitList->getNumInits() == 2) {
            std::string var1Name = InitList->getInit(0)->getSourceRange().getBegin().printToString(*Result.SourceManager);
            std::string var2Name = InitList->getInit(1)->getSourceRange().getBegin().printToString(*Result.SourceManager);
            mutatedCode = "auto tempVar = " + declaration.substr(0, declaration.find('=')) + ";\n";
            mutatedCode += "auto " + var1Name + " = tempVar.first;\n";
            mutatedCode += "auto " + var2Name + " = tempVar.second;\n";
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_Replace_Structured_Binding_With_Traditional_Assignment_394::MutatorASTConsumer_Replace_Structured_Binding_With_Traditional_Assignment_394::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclRefExpr declRefExpr;
    DeclarationMatcher matcher = declRefExpr().bind("DeclRefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}