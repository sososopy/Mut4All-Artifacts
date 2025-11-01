//source file
#include "../include/Alter_Function_Return_Types_In_Initializer_Lists_422.h"

// ========================================================================================================
#define MUT422_OUTPUT 1

void MutatorFrontendAction_422::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("initListFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (FD->getReturnType()->isIntegerType()) {
            auto returnType = FD->getReturnType().getAsString();
            auto newReturnType = returnType;
            std::string::size_type pos = newReturnType.find("std::initializer_list");
            if (pos != std::string::npos) {
                newReturnType.replace(pos, 21, "std::vector");
            }
            
            auto funcSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            std::string::size_type returnTypePos = funcSource.find(returnType);
            if (returnTypePos != std::string::npos) {
                funcSource.replace(returnTypePos, returnType.length(), newReturnType);
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcSource);
        }
    }
}

void MutatorFrontendAction_422::MutatorASTConsumer_422::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(returns(hasCanonicalType(asString("std::initializer_list")))).bind("initListFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}