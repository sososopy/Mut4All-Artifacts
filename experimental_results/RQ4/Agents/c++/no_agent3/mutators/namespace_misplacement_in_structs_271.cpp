//header file
#pragma once
#include "Mutator_base.h"

/**
 * Namespace_Misplacement_In_Structs_271
 */ 
class MutatorFrontendAction_271 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(271)

private:
    class MutatorASTConsumer_271 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_271(Rewriter &R) : TheRewriter(R) {}
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
        const NamespaceDecl *targetNamespace = nullptr;
    };
};

//source file
#include "../include/namespace_misplacement_in_structs_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
            return;
        
        if (!SD->isStruct() || !SD->isCompleteDefinition())
            return;
        
        if (targetNamespace == nullptr) {
            for (auto &decl : Result.Context->getTranslationUnitDecl()->decls()) {
                if (auto *NS = llvm::dyn_cast<NamespaceDecl>(decl)) {
                    targetNamespace = NS;
                    break;
                }
            }
        }

        if (targetNamespace) {
            std::string namespaceUsage = "\nnamespace using " + targetNamespace->getNameAsString() + "; // Misplaced\n";
            auto structSourceRange = SD->getSourceRange();
            std::string structContent = stringutils::rangetoStr(*(Result.SourceManager), structSourceRange);
            structContent.insert(structContent.find("{") + 1, namespaceUsage);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(structSourceRange), structContent);
        }
    }
}

void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}