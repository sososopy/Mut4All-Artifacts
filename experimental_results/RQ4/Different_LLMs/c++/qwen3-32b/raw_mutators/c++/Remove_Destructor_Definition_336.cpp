//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Destructor_Definition_336
 */ 
class MutatorFrontendAction_336 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(336)

private:
    class MutatorASTConsumer_336 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_336(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl*> usedClasses;
    };
};

//source file
#include "../include/Remove_Destructor_Definition_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        // Check if the type is a class
        const auto *recordType = VD->getType()->getAs<clang::RecordType>();
        if (!recordType)
            return;
        const auto *recordDecl = recordType->getDecl()->getAsCXXRecordDecl();
        if (!recordDecl)
            return;
        usedClasses.insert(recordDecl);
    } else if (auto *DD = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("destructorDecl")) {
        // Filter nodes in header files
        if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation()))
            return;
        // Check if has body
        if (!DD->hasBody())
            return;
        // Get the class
        const auto *classDecl = DD->getParent();
        if (!classDecl)
            return;
        // Check if class is in usedClasses
        if (usedClasses.find(classDecl) == usedClasses.end())
            return;
        // Get the class name
        std::string className = classDecl->getNameAsString();
        // Create the new declaration
        std::string newDecl = "~" + className + ";";
        newDecl = "/*mut336*/" + newDecl;
        // Replace the source range
        SourceLocation start = DD->getSourceRange().getBegin();
        SourceLocation end = DD->getSourceRange().getEnd();
        Rewrite.ReplaceText(SourceRange(start, end), newDecl);
    }
}

void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher varMatcher = varDecl(hasType(recordType())).bind("varDecl");
    DeclarationMatcher destructorMatcher = cxxDestructorDecl(hasBody()).bind("destructorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(destructorMatcher, &callback);
    matchFinder.matchAST(Context);
}