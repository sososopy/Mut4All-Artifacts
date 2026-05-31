//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Static_Undefined_Function_244
 */ 
class MutatorFrontendAction_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(244)

private:
    class MutatorASTConsumer_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_244(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Static_Undefined_Function_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        
        const auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl");
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        
        // Replace non-static method with static declaration
        auto body = MT->getBody();
        if (!body) return;
        
        clang::SourceRange range(MT->getBeginLoc(), body->getBeginLoc());
        std::string declText = stringutils::rangetoStr(*Result.SourceManager, range);
        std::string newDecl = "static " + declText + ";";
        
        Rewrite.ReplaceText(MT->getSourceRange(), newDecl);
        
        // Insert call to static function in constructor
        bool hasCtor = false;
        const clang::CXXConstructorDecl *ctor = nullptr;
        for (const auto *D : CL->decls()) {
            if (auto *Ctor = clang::dyn_cast<clang::CXXConstructorDecl>(D)) {
                hasCtor = true;
                ctor = Ctor;
                break;
            }
        }
        
        if (!hasCtor) {
            // Add new constructor
            std::string ctorName = CL->getNameAsString();
            std::string fnName = MT->getNameAsString();
            std::string newCtor = ctorName + "() { " + fnName + "(); }";
            Rewrite.InsertText(CL->getEndLoc(), newCtor, true, true);
        } else {
            if (ctor->hasBody()) {
                auto body = ctor->getBody();
                if (body) {
                    std::string call = MT->getNameAsString() + "();";
                    Rewrite.InsertText(body->getEndLoc(), call, true, true);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher matcher = 
        clang::ast_matchers::cxxMethodDecl(
            clang::ast_matchers::unless(clang::ast_matchers::decl::isStatic()),
            clang::ast_matchers::hasBody(clang::ast_matchers::stmt()),
            clang::ast_matchers::unless(clang::ast_matchers::cxxConstructorDecl()),
            clang::ast_matchers::ofClass(clang::ast_matchers::cxxRecordDecl().bind("structDecl"))
        ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}