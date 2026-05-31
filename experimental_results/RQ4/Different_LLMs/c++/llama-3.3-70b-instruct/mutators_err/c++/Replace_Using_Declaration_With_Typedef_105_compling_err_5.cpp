//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Using_Declaration_With_Typedef_105
 */ 
class MutatorFrontendAction_105 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_105>(TheRewriter);
    }

private:
    Rewriter TheRewriter;
    class MutatorASTConsumer_105 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_105(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Using_Declaration_With_Typedef_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecls")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 UD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string typedefDecl = "typedef ";
      if (UD->getQualifierLoc().getNestedNameSpecifier() == nullptr) {
        typedefDecl += UD->getNamedType()->getCanonicalType().getAsString();
      } else {
        typedefDecl += UD->getQualifierLoc().getNestedNameSpecifier()->getPrefix()->getAsString() + "::";
        typedefDecl += UD->getNamedType()->getCanonicalType().getAsString();
      }
      typedefDecl += " " + UD->getNameAsString() + ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), typedefDecl);
    }
}
  
void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingDecl().bind("UsingDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}