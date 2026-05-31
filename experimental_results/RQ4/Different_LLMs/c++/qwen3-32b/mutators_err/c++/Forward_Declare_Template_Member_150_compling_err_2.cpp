//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declare_Template_Member_150
 */ 
class MutatorFrontendAction_150 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(150)

private:
    class MutatorASTConsumer_150 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_150(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Forward_Declare_Template_Member_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classTemplate")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        std::string className = "Mut_ForwardDecl_150";
        SourceManager &SM = Rewrite.getSourceMgr();
        FileID mainFileID = SM.getMainFileID();
        if (mainFileID.isValid()) {
            FullSourceLoc loc(SM.getLocForStartOfFile(mainFileID), SM);
            if (loc.isValid()) {
                Rewrite.InsertText(loc, "class " + className + ";\n", true, true);
            }
        }

        FieldDecl *field = nullptr;
        for (auto *FD : CD->fields()) {
            field = FD;
            break;
        }
        if (!field)
            return;

        SourceRange typeRange = field->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;
        Rewrite.ReplaceText(typeRange, className);

        for (auto *method : CD->methods()) {
            if (method->isTemplated() && method->hasBody()) {
                Stmt *body = method->getBody();
                if (!body)
                    continue;

                std::string fieldName = field->getNameAsString();
                std::string insertCode = fieldName + " = " + className + "();";
                SourceLocation insertLoc = body->getBeginLoc().getLocWithOffset(1);
                if (insertLoc.isInvalid())
                    continue;
                Rewrite.InsertText(insertLoc, insertCode + "\n", true, true);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isClass(), clang::ast_matchers::isTemplated(), has(fieldDecl())).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}