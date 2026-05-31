//header file
#pragma once
#include "Mutator_base.h"

/**
 * ForwardDeclaredNestedUnion_229
 */ 
class MutatorFrontendAction_229 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(229)

private:
    class MutatorASTConsumer_229 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_229(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ForwardDeclaredNestedUnion_229.h"

// ========================================================================================================
#define MUT229_OUTPUT 1

void MutatorFrontendAction_229::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UnionDecl>("unionDecl")) {
        //Filter nodes in header files
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;
        //Get the source code text of target node
        std::string unionName = UD->getNameAsString();
        if (unionName.empty())
            return;

        // Generate a new name for the typedef union
        std::string newUnionName = "Mut_229";

        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation loc = UD->getBeginLoc();

        // Insert forward declaration
        std::string forwardDecl = "union " + unionName + ";";
        Rewrite.InsertTextBefore(loc, forwardDecl + "\n");

        // Insert the new typedef union
        std::string newUnion = "typedef union __attribute__((__transparent_union__)) {\n    union " + unionName + " " + unionName + ";\n} " + newUnionName + ";\n";
        Rewrite.InsertTextBefore(loc, newUnion);
    }
}
  
void MutatorFrontendAction_229::MutatorASTConsumer_229::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = unionDecl().bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}