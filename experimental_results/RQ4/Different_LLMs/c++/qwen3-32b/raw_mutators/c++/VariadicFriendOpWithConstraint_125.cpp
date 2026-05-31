//header file
#pragma once
#include "Mutator_base.h"

/**
 * VariadicFriendOpWithConstraint_125
 */ 
class MutatorFrontendAction_125 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(125)

private:
    class MutatorASTConsumer_125 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_125(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/VariadicFriendOpWithConstraint_125.h"

// ========================================================================================================
#define MUT125_OUTPUT 1

void MutatorFrontendAction_125::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;

        bool inExportedModule = false;
        DeclContext *ctx = recordDecl->getDeclContext();
        while (ctx) {
            if (auto *module = dyn_cast<clang::ModuleDecl>(ctx)) {
                if (module->isExported()) {
                    inExportedModule = true;
                    break;
                }
            }
            ctx = ctx->getParent();
        }
        if (!inExportedModule)
            return;

        std::string friendOp = R"(
    template<typename... Us> requires (... && std::is_same_v<Ts, Us>)
    friend bool operator==(S<Ts...>, S<Us...>) { return true; }
)";
        friendOp = "/*mut125*/" + friendOp;

        SourceLocation endLoc = recordDecl->getEndLoc();
        Rewrite.InsertText(endLoc, friendOp);

        std::string instantiation = R"(
template struct S<>;
)";
        Rewrite.InsertText(endLoc, instantiation);
    }
}
  
void MutatorFrontendAction_125::MutatorASTConsumer_125::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(templateParameterList())).bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}