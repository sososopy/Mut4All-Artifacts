//source file
#include "../include/Replace_Macro_With_Expression_216.h"

// ========================================================================================================
#define MUT216_OUTPUT 1

void MutatorFrontendAction_216::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("macroCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;

        if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {
            StructMemberExpr = ME;
        }

        if (StructMemberExpr && TargetMacroInfo) {
            std::string macroReplacement;
            llvm::raw_string_ostream stream(macroReplacement);
            StructMemberExpr->printPretty(stream, nullptr, Result.Context->getPrintingPolicy());

            std::string macroBody;
            for (auto token : TargetMacroInfo->tokens()) {
                macroBody += token.getIdentifierInfo() ? token.getIdentifierInfo()->getName().str() : token.getLiteralData();
            }

            size_t pos = macroBody.find("x");
            if (pos != std::string::npos) {
                macroBody.replace(pos, 1, stream.str());
            }
            
            pos = macroBody.find("y");
            if (pos != std::string::npos) {
                macroBody.replace(pos, 1, "5");
            }

            Rewrite.ReplaceText(CE->getSourceRange(), macroBody);
        }
    }
}

void MutatorFrontendAction_216::MutatorASTConsumer_216::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    auto macroMatcher = callExpr(callee(functionDecl(hasName("max"))), 
                                 hasArgument(1, memberExpr().bind("memberExpr")))
                        .bind("macroCall");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.matchAST(Context);
}