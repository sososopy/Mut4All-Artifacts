//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Base_Constructibility_Check_127
 */ 
class MutatorFrontendAction_127 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(127)

private:
    class MutatorASTConsumer_127 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_127(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Base_Constructibility_Check_127.h"

// ========================================================================================================
#define MUT127_OUTPUT 1

void MutatorFrontendAction_127::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        
        const CXXRecordDecl *recordDecl = CD->getParent();
        if (!recordDecl || !recordDecl->isCompleteDefinition())
            return;
        
        std::string baseClassName;
        for (const auto &base : recordDecl->bases()) {
            if (base.isVirtual()) continue;
            const Type *baseType = base.getType()->getTypePtr();
            if (const auto *baseClassDecl = baseType->getAsCXXRecordDecl()) {
                baseClassName = baseClassDecl->getNameAsString();
                break;
            }
        }
        if (baseClassName.empty()) return;
        
        RequiresExpr *RE = CD->getRequiresClause();
        if (!RE) return;
        
        bool otherTypeExists = false;
        for (const auto *D : Result.Context->getTranslationUnitDecl()->decls()) {
            if (const auto *RD = dyn_cast<CXXRecordDecl>(D)) {
                if (RD->getNameAsString() == "other_type") {
                    otherTypeExists = true;
                    break;
                }
            }
        }
        
        if (!otherTypeExists) {
            SourceLocation loc = Result.Context->getSourceManager().getLocForStartOfFile(
                Result.Context->getSourceManager().getMainFileID());
            Rewrite.InsertText(loc, "struct other_type {};\n", true, true);
        }
        
        SourceRange range = RE->getSourceRange();
        std::string requiresClauseText = stringutils::rangetoStr(*Result.SourceManager, range);
        std::string newRequiresClauseText = requiresClauseText;
        size_t pos = newRequiresClauseText.find(baseClassName);
        if (pos != std::string::npos) {
            newRequiresClauseText.replace(pos, baseClassName.length(), "other_type");
            Rewrite.ReplaceText(range, newRequiresClauseText);
        }
    }
}
  
void MutatorFrontendAction_127::MutatorASTConsumer_127::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(isTemplate(), hasRequiresClause()).bind("constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}