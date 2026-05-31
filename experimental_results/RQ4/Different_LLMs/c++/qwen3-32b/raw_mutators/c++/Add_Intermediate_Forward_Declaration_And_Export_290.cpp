//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Intermediate_Forward_Declaration_And_Export_290
 */ 
class MutatorFrontendAction_290 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(290)

private:
    class MutatorASTConsumer_290 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_290(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Intermediate_Forward_Declaration_And_Export_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *outerStruct = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("outerStruct")) {
        if (!outerStruct || !Result.Context->getSourceManager().isWrittenInMainFile(outerStruct->getLocation()))
            return;

        auto *nestedForwardDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("nestedForwardDecl");
        if (!nestedForwardDecl || !nestedForwardDecl->isForwardDeclaration())
            return;

        std::string nestedName = nestedForwardDecl->getNameAsString();
        const clang::CXXRecordDecl *nestedDef = nullptr;

        for (const auto *member : outerStruct->decls()) {
            if (const auto *decl = dyn_cast<clang::CXXRecordDecl>(member)) {
                if (decl->getNameAsString() == nestedName && decl->isThisDeclarationADefinition()) {
                    nestedDef = decl;
                    break;
                }
            }
        }

        if (!nestedDef)
            return;

        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation loc = nestedForwardDecl->getLocation();
        Rewrite.InsertTextAfter(loc, "\n    struct " + nestedName + ";  // Intermediate redundant declaration\n");

        SourceLocation defLoc = nestedDef->getLocation();
        std::string outerName = outerStruct->getNameAsString();
        Rewrite.InsertTextAfter(defLoc, "\nexport struct " + outerName + "::" + nestedName + " {};\n");
    }
}
  
void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        has(cxxRecordDecl(isForwardDeclaration()).bind("nestedForwardDecl"))
    ).bind("outerStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}