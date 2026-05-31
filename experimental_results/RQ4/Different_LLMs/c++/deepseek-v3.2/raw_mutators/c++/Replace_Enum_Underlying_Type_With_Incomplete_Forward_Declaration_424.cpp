//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_Underlying_Type_With_Incomplete_Forward_Declaration_424
 */ 
class MutatorFrontendAction_424 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(424)

private:
    class MutatorASTConsumer_424 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_424(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Enum_Underlying_Type_With_Incomplete_Forward_Declaration_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Check if the enum has an underlying type
      if (MT->getIntegerTypeSourceInfo()) {
        //Perform mutation on the source code text by applying string replacement
        //Remove underlying type and enumerators, leaving only "enum Name;"
        std::string enumName = MT->getNameAsString();
        std::string mutatedDeclaration;
        if (MT->isScoped()) {
          mutatedDeclaration = "enum class " + enumName + ";";
        } else {
          mutatedDeclaration = "enum " + enumName + ";";
        }
        mutatedDeclaration = "/*mut424*/" + mutatedDeclaration;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}