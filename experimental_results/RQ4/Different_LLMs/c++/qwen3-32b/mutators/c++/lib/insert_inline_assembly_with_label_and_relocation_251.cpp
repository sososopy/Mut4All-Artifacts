//source file
#include "../include/Insert_Inline_Assembly_With_Label_And_Relocation_251.h"

// ========================================================================================================
#define MUT251_OUTPUT 1

int MutatorFrontendAction_251::Callback::labelCounter = 0;

void MutatorFrontendAction_251::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *asmStmt = Result.Nodes.getNodeAs<clang::AsmStmt>("asmStmt")) {
        if (!asmStmt || !Result.Context->getSourceManager().isWrittenInMainFile(asmStmt->getBeginLoc()))
            return;

        int currentLabel = labelCounter++;
        std::string labelMut = ".Lmut_" + std::to_string(currentLabel);
        std::string labelRef = ".Lref_" + std::to_string(currentLabel);
        std::string asmCode = "__asm__(\""+ labelMut +" : movw r2, #:lower16:"+ labelRef +" - "+ labelMut +"\");";

        Rewrite.ReplaceText(asmStmt->getSourceRange(), asmCode);
    } else if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!funcDecl || !Result.Context->getSourceManager().isWrittenInMainFile(funcDecl->getLocation()))
            return;

        if (auto *body = funcDecl->getBody()) {
            if (auto *compoundStmt = dyn_cast<clang::CompoundStmt>(body)) {
                int currentLabel = labelCounter++;
                std::string labelMut = ".Lmut_" + std::to_string(currentLabel);
                std::string labelRef = ".Lref_" + std::to_string(currentLabel);
                std::string asmCode = "__asm__(\""+ labelMut +" : movw r2, #:lower16:"+ labelRef +" - "+ labelMut +"\");";

                SourceLocation loc = compoundStmt->getLBracLoc();
                Rewrite.InsertText(loc, asmCode + "\n", true, true);
            }
        }
    }
}

void MutatorFrontendAction_251::MutatorASTConsumer_251::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher1 = asmStmt().bind("asmStmt");
    DeclarationMatcher matcher2 = functionDecl(hasBody(compoundStmt())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}