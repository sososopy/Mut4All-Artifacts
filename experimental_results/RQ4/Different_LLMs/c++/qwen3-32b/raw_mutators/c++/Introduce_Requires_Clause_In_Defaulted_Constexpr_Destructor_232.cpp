//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Requires_Clause_In_Defaulted_Constexpr_Destructor_232
 */ 
class MutatorFrontendAction_232 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(232)

private:
    class MutatorASTConsumer_232 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_232(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Requires_Clause_In_Defaulted_Constexpr_Destructor_232.h"

// ========================================================================================================
#define MUT232_OUTPUT 1

void MutatorFrontendAction_232::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Dtor = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("dtor")) {
        // Filter nodes in header files
        if (!Dtor || !Result.Context->getSourceManager().isWrittenInMainFile(Dtor->getLocation()))
            return;

        auto *ClassDecl = Dtor->getParent();
        if (!ClassDecl || !ClassDecl->isClass() || !ClassDecl->getTemplateParameters())
            return;

        auto TemplateParams = ClassDecl->getTemplateParameters();
        if (TemplateParams->size() < 1)
            return;

        auto FirstParam = TemplateParams->getParam(0);
        if (!FirstParam)
            return;

        std::string TName = FirstParam->getNameAsString();

        // Generate the type alias
        std::string enableAlias = "template<typename U> using Enable = std::enable_if_t<std::is_same_v<U, " + TName + ">>;\n";

        // Insert the type alias before the destructor
        SourceLocation DtorLoc = Dtor->getBeginLoc();
        Rewrite.InsertTextBefore(DtorLoc, enableAlias);

        // Modify the destructor declaration
        std::string className = ClassDecl->getNameAsString();
        std::string newDtorDecl = "constexpr ~" + className + "() requires (Enable<" + TName + ">()) = default;";
        newDtorDecl = "/*mut232*/" + newDtorDecl;

        // Replace the original destructor declaration
        Rewrite.ReplaceText(Dtor->getSourceRange(), newDtorDecl);
    }
}

void MutatorFrontendAction_232::MutatorASTConsumer_232::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl(ofClass(classTemplateDecl())).bind("dtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}