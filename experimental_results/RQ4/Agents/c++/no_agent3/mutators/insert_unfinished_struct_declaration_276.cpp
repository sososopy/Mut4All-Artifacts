//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unfinished_Struct_Declaration_276
 */ 
class MutatorFrontendAction_276 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(276)

private:
    class MutatorASTConsumer_276 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_276(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_276::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SR = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        //Filter nodes in header files
        if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(SR->getLocation()))
            return;
        if (!SR->isCompleteDefinition())
            return;
        //Get the source code text of target node
        std::string structName = "UnfinishedStruct";
        std::string insertion = "\n    struct " + structName + "; /*mut276*/\n";
        //Perform mutation on the source code text by applying string replacement
        auto structRange = SR->getSourceRange();
        std::string structContent = stringutils::rangetoStr(*(Result.SourceManager), structRange);
        if (structContent.rfind('}') != std::string::npos)
            structContent.insert(structContent.rfind('}'), insertion);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(structRange), structContent);
    }
}
  
void MutatorFrontendAction_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}