//source file
#include "../include/Insert_Extern_Shadow_Variable_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        SmallVector<const clang::VarDecl*, 4> vars;
        for (const auto *Param : FD->parameters()) {
            vars.push_back(cast<const clang::VarDecl>(Param));
        }

        if (FD->getBody()) {
            for (const auto *Stmt : FD->getBody()->children()) {
                if (auto *DS = dyn_cast<clang::DeclStmt>(Stmt)) {
                    for (const auto *D : DS->decls()) {
                        if (auto *VD = dyn_cast<clang::VarDecl>(D)) {
                            vars.push_back(VD);
                        }
                    }
                }
            }
        }

        if (vars.empty())
            return;

        const clang::VarDecl *VD = vars[0];
        clang::QualType type = VD->getType();
        std::string typeName = type.getAsString();
        std::string varName = VD->getNameAsString();

        std::string externDecl = "extern " + typeName + " " + varName + ";";

        SourceLocation insertLoc = FD->getBody()->getEndLoc();
        Rewrite.InsertText(insertLoc, externDecl + "\n", true, true);
    }
}

void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}