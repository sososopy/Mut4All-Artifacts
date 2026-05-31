//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Member_Init_List_122
 */ 
class MutatorFrontendAction_122 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(122)
private:
    class MutatorASTConsumer_122 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_122(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Member_Init_List_122.h"

// ========================================================================================================
#define MUT122_OUTPUT 1

void MutatorFrontendAction_122::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        // Filter nodes in header files
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if (DL->isLambda())
            return;
        if (!DL->isCompleteDefinition())
            return;
        // Get the source code of the struct
        auto originalCode = stringutils::rangetoStr(*Result.SourceManager, DL->getSourceRange());
        string structName = DL->getNameAsString();
        if (structName.empty())
            return;
        // Find a member with incomplete type
        for (auto *FD : DL->fields()) {
            if (FD->getType()->isIncompleteType()) {
                string memberName = FD->getNameAsString();
                if (memberName.empty())
                    continue;
                // Generate the constructor with malformed initializer
                string constructor = structName + "() : " + memberName + "{ ( { } };\n";
                // Insert the constructor before the closing brace
                size_t endPos = originalCode.rfind('}');
                if (endPos != string::npos) {
                    originalCode.insert(endPos, constructor);
                    // Replace the original code with the mutated one
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), originalCode);
                    break;
                }
            }
        }
    }
}

void MutatorFrontendAction_122::MutatorASTConsumer_122::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(has(fieldDecl(hasType(isIncompleteType())))).bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}